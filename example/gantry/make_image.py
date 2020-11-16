#!/usr/local/bin/python3

import os, sys, time, argparse
import math, numpy
from ROOT import TFile, TTree, TH1D, TH2D, TVector3, TCanvas, TArrow, TAxis, TGraph
from ROOT import gStyle, gROOT
from ROOT import gErrorIgnoreLevel, kFatal

def usage():

    parser = argparse.ArgumentParser( description='Make Detector Image from g4tree',\
        formatter_class=argparse.ArgumentDefaultsHelpFormatter )

    parser.add_argument( 'file', metavar='FILE', type=str, nargs='+', help='a file of g4tree' )

    parser.add_argument( '--e1', type=float, default=0.0, help='beginning of energy window [keV]' )
    parser.add_argument( '--e2', type=float, default=1000.0, help='end of energy window [keV]' )

    parser.add_argument( '--outdir', default='.', help='output directory' )
    parser.add_argument( '--tree-name', default='g4tree', help='name of tree' )

    parser.add_argument( '--printfreq', metavar='FREQ', type=int, default=1000,\
        help='frequency to print progress' )
    parser.add_argument( '--verbose', action='count', default=0,\
        help='level of print message' )

    return parser

class single_event :

    class single_hit :
        def __init__(self) :
            self.detid = 0
            self.edep = 0.0
            self.strip_x = 0
            self.strip_y = 0
            self.merged_hits = []

        # def array(self) :
        #     return numpy.array( [self.detid, self.edep, self.strip_x, self.strip_y] )

        def merge(self,other) :
            self.detid = other.detid
            self.edep += other.edep
            self.merged_hits.append( other )

        def is_same_pixel(self,other) :
            # print('x=',self.strip_x, other.strip_x )
            div_x = self.strip_x - other.strip_x
            div_y = self.strip_y - other.strip_y
            if div_x == 0 and div_y == 0 :
                return True
            for hit in self.merged_hits :
                if hit.is_same_pixel( other ) :
                    return True
            return False

        def is_adjacent_pixel(self,other) :

            div_x = self.strip_x - other.strip_x
            div_y = self.strip_y - other.strip_y
            if abs(div_x) + abs(div_y) == 1 :
                return True
            for hit in self.merged_hits :
                if hit.is_adjacent_pixel( other ) :
                    return True
            return False


    def __init__(self,input_tree,args):
        self.input_tree = input_tree
        self.args = args
        self.current_entry = -1
        self.nentries = input_tree.GetEntries()
        self.tree_name = input_tree.GetName()

        self.eventID = 0
        self.nhits = 0
        self.hit_pattern = 0
        self.etotal = 0.0
        self.detid = []
        self.material = []
        self.strip_x = []
        self.strip_y = []
        self.edep  = []
        self.pos_x = []
        self.pos_y = []
        self.pos_z = []
        self.proc_name = []
        self.part_name = []

        self.hits_buffer = []
        # self.merged_hits = []

        self.merged_nhits = 0
        self.merged_detid = []
        self.merged_edep = []
        self.merged_strip_x = []
        self.merged_strip_y = []

    def read(self,entry=-1) :

        if entry < 0 :
            self.current_entry += 1
        else :
            self.current_entry = entry

        if self.current_entry >= self.nentries :
            return False

        self.input_tree.GetEntry( entry )

        self.eventID = self.input_tree.eventID
        self.nhits   = self.input_tree.nhits
        self.etotal  = self.input_tree.etotal
        self.detid   = numpy.array( self.input_tree.detid.data() )
        self.edep    = numpy.array( self.input_tree.edep.data() )
        self.strip_x = numpy.array( self.input_tree.strip_x.data() )
        self.strip_y = numpy.array( self.input_tree.strip_y.data() )
        self.pos_x   = numpy.array( self.input_tree.pos_x.data() )
        self.pos_y   = numpy.array( self.input_tree.pos_y.data() )
        self.pos_z   = numpy.array( self.input_tree.pos_z.data() )

        self.read_hits()
        self.merge_signals()

        return True

    def split_string(self,list_of_string) :
        proc_list = []
        for proc in list_of_string.split(':') :
            if proc != '':
                proc_list.append( proc )
        return proc_list

    def read_hits(self) :
        self.hits_buffer = []

        for i in range(self.nhits) :
            hit = self.single_hit()
            hit.detid   = self.detid[i]
            hit.edep    = self.edep[i]
            hit.strip_x = self.strip_x[i]
            hit.strip_y = self.strip_y[i]
            self.hits_buffer.append( hit )

        self.hits_buffer = numpy.array( self.hits_buffer )

    def merge_signals(self) :

        index_range = range(self.nhits)
        reversed_range = reversed( index_range )

        while True :

            nhits_remained = len(self.hits_buffer)

            if nhits_remained <= 1 :
                return

            for i in index_range :

                if nhits_remained-1 <= i :
                    continue

                hit1 = self.hits_buffer[i]

                for j in reversed_range :
                    if j < i+1 or nhits_remained <= j:
                        continue

                    hit2 = self.hits_buffer[j]

                    if hit1.is_same_pixel( hit2 )\
                        or hit1.is_adjacent_pixel( hit2 ) :

                        hit1.merge( hit2 )
                        numpy.delete( self.hits_buffer, j )
                        continue

            if nhits_remained == len(self.hits_buffer) :
                break

        self.merged_nhits   = len(self.hits_buffer)
        self.merged_detid   = numpy.zeros(self.merged_nhits)
        self.merged_edep    = numpy.zeros(self.merged_nhits)
        self.merged_strip_x = numpy.zeros(self.merged_nhits)
        self.merged_strip_y = numpy.zeros(self.merged_nhits)

        for i in range( self.merged_nhits ) :
            self.merged_detid[i]   = self.hits_buffer[i].detid
            self.merged_edep[i]    = self.hits_buffer[i].edep
            self.merged_strip_x[i] = self.hits_buffer[i].strip_x
            self.merged_strip_y[i] = self.hits_buffer[i].strip_y

class result_histogram :

    def __init__(self,args) :

        self.nlayers = 8
        self.nstrips = 128
        # self.layer_halfwidth = 16.0
        # self.strip_pitch = self.layer_halfwidth*2/self.nstrips

        self.ewindow_bgn = args.e1
        self.ewindow_end = args.e2

        self.dict_of_images = {}
        self.dict_of_histos = {}

        for detid in range(self.nlayers) :
            hname = 'image_detid{:03d}'.format( detid )
            htitle = hname + ';X-strip;Y-strip'

            image = TH2D( hname, htitle,\
                self.nstrips, -0.5, self.nstrips-0.5,\
                self.nstrips, self.nstrips-0.5, self.nstrips*2-0.5 )

            self.dict_of_images[ detid ] = image

            hname = hname.replace( 'image', 'histo' )
            htitle = htitle.replace( 'image', 'histo' )

            histo = TH1D( hname, htitle, 5000, -0.5, 4999.5 )
            self.dict_of_histos[ detid ] = histo

    def in_energy_window(self,e) :
        return self.ewindow_bgn <= e and e <= self.ewindow_end

    def fill(self,detid,x,y,e) :
        if self.in_energy_window(e) :
            self.dict_of_images[ detid ].Fill( x, y, e )
            self.dict_of_histos[ detid ].Fill( e )

class main_analysis :

    def __init__(self,input_tree,args) :
        self.input_tree = input_tree
        self.nentries = input_tree.GetEntries()
        self.args = args
        self.print_frequency = args.printfreq
        self.verbose = args.verbose

        self.event = single_event( self.input_tree, self.args )
        self.histo = result_histogram( self.args )

    def event_routine(self,entry) :

        entry = entry[0]

        if entry%self.print_frequency == 0 :
            print( '{:d}/{:d}({:5.2f}%)'\
                .format( entry, self.nentries,\
                float( entry/self.nentries*100.0 ) ) )

        if self.event.read( entry ) == False :
            return

        # for i in range(self.event.nhits) :
            # self.histo.fill( self.event.detid[i], self.event.strip_x[i], \
            #     self.event.strip_y[i], self.event.etotal )
        for i in range(self.event.merged_nhits) :
            self.histo.fill( self.event.merged_detid[i], self.event.merged_strip_x[i], \
                self.event.merged_strip_y[i], self.event.merged_edep[i] )

def do_analysis(file_name,args) :

    file = TFile( file_name )
    if file == None or file.IsZombie() == True :
        print( '***Error*** :', file_name, 'is not found' )
        return
    print( file_name, 'is opened' )

    tree_name  = args.tree_name
    input_tree = file.Get( tree_name )

    if input_tree == None :
        print( '***Error*** :', tree_name, 'is not found in', file_name )
        return
    print( tree_name, 'is loaded from', file_name )

    base = os.path.basename( file_name )
    out_name = args.outdir+'/'+base.replace( '.root', '_ana.root' )
    if out_name == file_name :
        print( '***Error*** :', 'extension of', file_name, 'is not ".root"' )
        return
    out_file = TFile( out_name, 'recreate' )
    if out_file == None or out_file.IsZombie() == True :
        print( '***Error*** :', 'opening', out_name, 'is failed' )
        return
    print( out_name, 'is generated' )

    analysis = main_analysis( input_tree, args )
    entries = numpy.array( [ range( analysis.nentries ) ] ).T
    numpy.apply_along_axis( analysis.event_routine, axis=1, arr=entries )

    c = TCanvas( 'c', 'c', 400, 400 )

    out_file.cd()
    for histo in analysis.histo.dict_of_histos.values() :
        histo.Write()
    for image in analysis.histo.dict_of_images.values() :
        image.Write()
    out_file.Close()

if __name__ == '__main__':

    parser = usage()
    args = parser.parse_args()

    os.makedirs( args.outdir, exist_ok=True )
    gROOT.SetBatch( True )

    for file in args.file:
        start = time.time()
        do_analysis( file, args )
        end = time.time()
        print( end - start, '[sec]' )
