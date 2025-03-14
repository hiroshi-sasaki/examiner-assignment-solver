# スケジュール作成ツール

## 概要

系で行われるイベントのスケジュールを作成するツールである。以下の 5 つについてスケジュールを作成することができる。各イベントについては各々のフォルダにある `README.md` に詳細がある。

- 研究プロジェクト `research_project/`
- 構想発表会 `concept_presentation/`
- 中間発表 `intermediate_examination/`
- 学士特定課題研究 `bachelor_presentation/`
- 修論発表 `master_presentation/`

## ビルド方法

以下のコマンドを上から実行することで `build/solver` という実行ファイルが作成される。

```bash
$ mkdir build
$ cd build
$ cmake ..
$ make
```