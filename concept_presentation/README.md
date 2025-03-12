# 構想発表会

## 使用方法

コマンド引数の第 1 引数には `concept_presentation` 、第 2 引数には教員の基本情報の入ったCSVファイル名、第 3 引数には教員のスケジュールが入ったCSVファイル名、第 4 引数には学生の情報の入ったCSVファイル名を与える。

以下に例をあげる。ファイル名はそれぞれ `professor_base_info.csv`, `professor.csv`, `student.csv` としている。

```
$ ./build/solver concept_presentation professor_base_info.csv professor.csv student.csv
```

## CSVの中身

以下のようなフォーマットのCSV形式で与える。

### 教員

| 列名 | 説明 |
| ---- | ---- |
| ご自身のお名前をお選びください | 教員氏名 |
| 1枠目 | 1枠目のラベル名 |
| 2枠目 | 2枠目のラベル名 |

```
ご自身のお名前をお選びください,1枠目,2枠目
山田太郎,OK,NG
huga,OK,OK,
aaa,NG,NG
.
.
.
```

### 学生

| 列名 | 説明 |
| ---- | ---- |
| 学籍番号／Student ID  No.,主指導教員を選んでください／Choose your main academic supervisor. | 学籍番号 |
| 片方のみの場合は行ける方の数字 | 片方の枠しか行けない場合に入力される（ `1` or `2`） |

```
学籍番号／Student ID  No.,主指導教員を選んでください／Choose your main academic supervisor.,片方のみの場合は行ける方の数字
99B99998,山田太郎,田中太郎,,
99B99999,hoge,huga,2,
99B99997,aaa,bbb,1,
.
.
.
```