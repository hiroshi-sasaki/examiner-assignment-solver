# スケジュール作成ツール

## 概要

系で行われるイベントのスケジュールを作成するツールである。以下の 5 つについてスケジュールを作成することができる。各イベントについては `src/events` 以下の各々のフォルダにある `README.md` に詳細がある。

- 研究プロジェクト `research_project`
- 構想発表会 `concept_presentation`
- 中間発表 `intermediate_examination`
- 学士特定課題研究 `bachelor_presentation`
- 修論発表 `master_presentation`

## ビルド方法

以下のコマンドを上から実行することで `build/solver` という実行ファイルが作成される。

```bash
$ mkdir build
$ cd build
$ cmake ..
$ make
```

## Docker

### Docker イメージの作成

```bash
$ docker build --build-arg UNAME=$(id -un) --build-arg GNAME=$(id -gn) --build-arg UID=$(id -u) --build-arg GID=$(id -g) -t examiner-assignment-solver -f Dockerfile .
```

### Docker を用いたビルド方法

```bash
$ docker run --rm -v $(pwd):/home/$(id -un)/workspace examiner-assignment-solver bash ./build_linux.sh
```

## 必須ファイル

`base/` に以下のファイルを置いておく必要がある。適宜更新しておくこと。

- 教員の基本情報を扱うファイル `base/professor_base_info.csv`
- 教員のCSVのヘッダの表記ゆれを解消するためのファイル `base/professor_label.csv`
- 学生のCSVのヘッダの表記ゆれを解消するためのファイル `base/student_label.csv`
