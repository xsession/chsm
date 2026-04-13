// CHSM — Typst Document Template
// Professional enterprise-grade PDF output

#let project(
  title: "",
  subtitle: "",
  version: "",
  author: "",
  date: datetime.today().display(),
  logo: none,
  body,
) = {
  // ── Page setup ──────────────────────────────────────────────
  set page(
    paper: "a4",
    margin: (top: 3cm, bottom: 2.5cm, left: 2.5cm, right: 2.5cm),
    header: context {
      if counter(page).get().first() > 1 {
        set text(8pt, fill: luma(120))
        grid(
          columns: (1fr, 1fr),
          align(left, title),
          align(right, [v#version]),
        )
        line(length: 100%, stroke: 0.4pt + luma(200))
      }
    },
    footer: context {
      set text(8pt, fill: luma(120))
      line(length: 100%, stroke: 0.4pt + luma(200))
      v(4pt)
      grid(
        columns: (1fr, 1fr),
        align(left, [#author]),
        align(right, counter(page).display("1 / 1", both: true)),
      )
    },
  )

  // ── Typography ──────────────────────────────────────────────
  set text(font: "Libertinus Serif", size: 10.5pt, lang: "en")
  set par(justify: true, leading: 0.65em)
  set heading(numbering: "1.1.1")

  show heading.where(level: 1): it => {
    pagebreak(weak: true)
    v(1cm)
    set text(18pt, weight: "bold", fill: rgb("#1a3c5e"))
    block(it)
    v(0.5cm)
  }

  show heading.where(level: 2): it => {
    v(0.6cm)
    set text(14pt, weight: "bold", fill: rgb("#2a5f8f"))
    block(it)
    v(0.3cm)
  }

  show heading.where(level: 3): it => {
    v(0.4cm)
    set text(12pt, weight: "bold")
    block(it)
    v(0.2cm)
  }

  // ── Code blocks ─────────────────────────────────────────────
  show raw.where(block: true): it => {
    set text(9pt)
    block(
      fill: luma(245),
      inset: 10pt,
      radius: 3pt,
      width: 100%,
      stroke: 0.5pt + luma(220),
      it,
    )
  }

  show raw.where(block: false): it => {
    box(
      fill: luma(240),
      inset: (x: 3pt, y: 0pt),
      outset: (y: 3pt),
      radius: 2pt,
      it,
    )
  }

  // ── Tables ──────────────────────────────────────────────────
  set table(
    stroke: 0.5pt + luma(200),
    inset: 6pt,
  )

  // ── Cover page ──────────────────────────────────────────────
  page(
    margin: (top: 0cm, bottom: 0cm, left: 0cm, right: 0cm),
    header: none,
    footer: none,
  )[
    #box(
      width: 100%,
      height: 100%,
      fill: rgb("#1a3c5e"),
    )[
      #align(center + horizon)[
        #block(inset: 2cm)[
          #text(36pt, weight: "bold", fill: white, title)
          #v(0.5cm)
          #text(18pt, fill: rgb("#8ab4d6"), subtitle)
          #v(1.5cm)
          #line(length: 40%, stroke: 1pt + rgb("#8ab4d6"))
          #v(1cm)
          #text(14pt, fill: rgb("#c0d8ec"), [Version #version])
          #v(0.5cm)
          #text(12pt, fill: rgb("#c0d8ec"), date)
          #v(0.5cm)
          #text(12pt, fill: rgb("#c0d8ec"), author)
        ]
      ]
    ]
  ]

  // ── Table of contents ───────────────────────────────────────
  page(header: none)[
    #heading(outlined: false, numbering: none, [Table of Contents])
    #outline(indent: 1.5em, depth: 3)
  ]

  // ── Body ────────────────────────────────────────────────────
  body
}
