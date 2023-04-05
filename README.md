# DIM2XDF

DIM image to XDF image converter for X680x0/Python

DIMファイルをXDFファイルに変換します。

---

### インストール方法 (X680x0/Human68k版)

DM2XDFxxx.ZIP をダウンロードして展開し、DIM2XDF.X をパスの通ったディレクトリに置きます。

---

### インストール方法 (Python版)

    pip install git+https://github.com/tantanGH/dim2xdf.git

[Windowsユーザ向けPython導入ガイド](https://github.com/tantanGH/distribution/blob/main/windows_python_for_x68k.md)

---

### 使用方法

    dim2xdf [オプション] <DIMファイル名> [XDFファイル名]

      -h    ... ヘルプメッセージを表示します

Human68k版は、XDFファイル名を省略すると自動的にDIMファイル名の拡張子を変更したものになります。

---

### Special Thanks

* xdev68k thanks to ファミべのよっしんさん
* HAS060.X on run68mac thanks to YuNKさん / M.Kamadaさん / GOROmanさん
* HLK301.X on run68mac thanks to SALTさん / GOROmanさん

---

### History

* 0.1.0 (2023/04/05) ... Human68k版追加
* 0.0.1 (2022/12/24) ... Python版