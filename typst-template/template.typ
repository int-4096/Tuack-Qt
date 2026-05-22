// ============================================================
// OI Problem Statement Template for Typst
// Designed for use with Tuack-Qt
// ============================================================

// ---------- 页面设置 ----------
#set page(
  paper: "a4",
  margin: (x: 2.5cm, y: 2.5cm),
  numbering: "1",
)

// ---------- 字体设置（支持 fallback，优先使用打包字体） ----------
// 你可以通过 --font-path 将自定义字体目录传给 typst
// 这里假设打包字体名称是 "FiraGO" 或 "Noto Sans CJK SC"
#let main-font = (
  "FiraGO",                 // 如果你将来打包了 FiraGO 或其他中文字体
  "Noto Sans CJK SC",
  "Microsoft YaHei",
  "PingFang SC",
  "SimHei",
)
#let mono-font = (
  "Sarasa Mono SC",
  "Fira Code",
  "Courier New",
)

#set text(font: main-font, size: 11pt)

// 标题：更大更粗
#show heading: set text(font: main-font, weight: "bold")
#show heading.where(level: 1): set text(size: 20pt)
#show heading.where(level: 2): set text(size: 16pt)
#show heading.where(level: 3): set text(size: 13pt)

// 代码块 / 行内代码
#show raw: set text(font: mono-font, size: 10pt)
#show raw.where(block: true): set block(spacing: 0.6em, above: 0.5em, below: 0.5em)
#show raw: it => {
  if it.block {
    box(
      inset: 8pt,
      width: 100%,
      stroke: rgb(220, 220, 220),
      fill: rgb(248, 248, 248),
      radius: 4pt,
      it,
    )
  } else {
    text(
      font: mono-font,
      size: 0.9em,
      fill: rgb(200, 40, 40),
      it.text,
    )
  }
}

// ---------- 常用题面元素函数 ----------
// 1. 题目基本信息
#let problem-info(title: "", time-limit: "1s", memory-limit: "256 MiB") = [
  #set align(center)
  #text(size: 24pt, weight: "bold", title)
  #v(8pt)
  #text(size: 11pt, [时间限制：#time-limit]  [空间限制：#memory-limit])
  #v(16pt)
]

// 2. 样例展示（支持多个样例）
#let samples(..pairs) = {
  for (i, (input, output)) in pairs.enumerate() {
    let idx = i + 1
    [#strong("样例", [ #idx ]) #line(length: 100%)]
    #grid(
      columns: (1fr, 1fr),
      column-gutter: 12pt,
      [
        #set text(size: 10pt)
        #strong("输入")
        #raw(text: input, block: true)
      ],
      [
        #set text(size: 10pt)
        #strong("输出")
        #raw(text: output, block: true)
      ],
    )
    #v(8pt)
  }
}

// 3. 自定义分节风格（描述、输入格式、输出格式等）
#let section(title) = [
  #heading(title, level: 2)
]

// ---------- 以下为示例使用（用户可删除） ----------
// #problem-info(
//   title: "两数之和",
//   time-limit: "1s",
//   memory-limit: "256 MiB",
// )

// #section("题目描述")
// 给定两个整数 $a$ 和 $b$，求它们的和。

// #section("输入格式")
// 一行两个整数 $a,b$。

// #section("输出格式")
// 一个整数，代表 $a+b$。

// #section("样例")
// #samples(
//   ("1 2", "3"),
//   ("10 20", "30"),
// )

// #section("数据范围")
// $-10^9 \le a,b \le 10^9$。