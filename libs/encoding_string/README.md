# はじめに

`encoding_string`は、文字コードのエンコーディング情報を持つ文字列クラスである。

## 選択可能な文字コード

`encoding_string`クラスでは、以下の文字コードを選択可能だ。

| 文字コード         | 説明 |
|--------------------|------|
| UTF-16             | 16ビット(2バイト)で1文字を表現する。<br/>後のUnicode領域の拡張により16ビットで全ての文字は表現できなくなった。そのため、稀に2文字分の領域を使用する「サロゲートペア(surrogate pair)」と呼ばれる種類の文字が現れることがある。<br/>WindowsのUnicode APIはUTF-16を使用する。 |
| UTF-8              | 複数バイトで1文字を表現する。<br/>ASCII互換の文字コードで、ASCII部分(アルファベット、数字、少しの記号と制御文字)は1バイトかつASCIIのそれと同じビット列で表現する。ASCIIを越える部分は、1バイトから最大4バイトで1文字を表現する。<br/>6バイト文字の領域も仕様上は存在するが、過去の他の文字コードとの互換性のためであり、現在は使われていない。 |
| UTF-32             | 32ビット(4バイト)で1文字を表現する。<br/>UTF-16とは違い、全ての文字が32ビット領域内で表現可能なため、サロゲートペアは存在しない。 |
| システム文字コード | この文字コードは、標準出力の文字コードである。<br/>たとえばWindowsのコマンドプロンプトは、デフォルト設定ではUTF-8文字コードの文字を出力できない。そのため、Windowsではシステム文字コードであるCP932(Shift_JISのMicrosoft拡張)に変換してから出力する必要がある。また、Microsoft Visual C++においては文字コード指定なしの文字列リテラルもまた、CP932である。<br/>その他の環境において、システム文字コードはUTF-8としている。 |

この表における文字コードの順番は、システム文字コードを除いて歴史順である。UTF-16が最も古く、UTF-32が最も新しい。


## 名前空間
`encoding_string`、エンコーディングの種類、その他関数は、`shand`名前空間以下で定義される。


## ドキュメント目次

- [基本的な使い方](doc/basic_usage.md)
- [ビルド方法](doc/build.md)
- [リファレンス](doc/reference.md)
- 設計根拠
- [将来の予定](doc/future_plan.md)


