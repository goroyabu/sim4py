# sim4py
Monte-Carlo Simulator using Geant4 with Python interface

## Concept

 - python で条件設定を変更可能なモンテカルロシミュレーションソフト
   - Geant4 + pybind11
 - 出力結果は基本的に Geant4 の出力をそのまま記録する
   - 形式は ROOT の TTree
   - ピクセル番号の割り振りなどは本ソフト内で行う
 - 複雑なジオメトリや特殊な物理リストの記述は C++ コードで行う
   - 改変して使える Example が付属

# Installation & Tutorial

## Requirement

 - Python 3.7.7
 - CMake 3.17.3
 - Geant4 10.5.1
 - pybind11 2.5.0

mac の場合、すべて Homebrew で入れられます。
```
 $ brew install python
 $ brew install cmake
 $ brew install geant4
 $ brew install pybind11
```

## Installation

以下のコマンドでインストールができます。

```
 $ git clone https://github.com/goroyabu/sim4py
 $ pip3 install sim4py/
```
pip 以外の python パッケージ管理システムを使っている場合はどうすればいいのかわかりません。

```
 $ python3
>>> import sim4py
```

として、何も言われなかったら、大丈夫です。

## Tutorial

あとで書きます。
