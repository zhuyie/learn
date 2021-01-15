---
marp: true
theme: gaia
backgroundColor: #fff
backgroundImage: url('https://marp.app/assets/hero-background.jpg')
---
<!-- _class: lead -->
# Unicode: not only a Charset
zhuyie
zhuyie@gmail.com

---
# Terminology
* A **character** is a minimal unit of text that has semantic value.
* A **character set** is a collection of characters that might be used by multiple languages. Example: The Latin character set is used by English and most European languages.
* A **coded character set** is a character set in which each character corresponds to a unique number.
* A **code point** of a coded character set is any allowed value in the character set or code space.
* A **code space** is a range of integers whose values are code points.

---
# Terminology (cont.)
* A **code unit** is the unit of storage of a part of an encoded code point. In UTF-8 this means 8-bits, in UTF-16 this means 16-bits. A single code unit may represent a full code point, or part of a code point.
* **character encoding form**: Mapping from a character set definition to the actual code units used to represent the data.
* **character encoding scheme**: A character encoding form plus byte serialization. There are seven character encoding schemes in Unicode: UTF-8, UTF-16, UTF-16BE, UTF-16LE, UTF-32, UTF-32BE, and UTF-32LE.

---
# Terminology (cont. 2)
* A **language** is a structured system of communication.
* A **script** is a collection of letters and other written signs used to represent textual information in one or more writing systems. For example, Russian is written with a subset of the Cyrillic script; Ukranian is written with a different subset. The Japanese writing system uses several scripts.
* A **writing system** is a set of rules for using one or more scripts to write a particular language. Examples include the American English writing system, the British English writing system, the French writing system, and the Japanese writing system.

---
# Terminology (cont. 3)
* A **grapheme/grapheme cluster** is a sequence of one or more code points that are displayed as a single, graphical unit that a reader recognizes as a single element of the writing system. For example, both **a** and **ä** are graphemes, but they may consist of multiple code points.
* A **glyph** is an image, usually stored in a font (which is a collection of glyphs), used to represent graphemes or parts thereof.
 
---
<!-- paginate: true -->
# What is Unicode?
[Unicode](https://en.wikipedia.org/wiki/Unicode) is an information technology (IT) **standard for the consistent encoding, representation, and handling of text expressed** in most of the world's writing systems. The standard is maintained by the **Unicode Consortium**, and as of March 2020, there is a total of 143,859 characters, with Unicode 13.0 covering 154 modern and historic scripts, as well as multiple symbol sets and emoji. The character repertoire of the Unicode Standard is **synchronized with ISO/IEC 10646**, and both are code-for-code identical.

---
# A Brief History of Unicode
* 1984年，一个工作组开始准备ISO/IEC 10646，试图解决传统字符集的各类问题。这个工作组的正式名称是：ISO/IEC JTC1/SC2/WG2 (that's "ISO/IEC Joint Technical Committee #1 [Information Technology], Subcommittee #2 [Coded Character Sets], Working Group #2 [Multioctet codes]"), or just "**WG2**" for short.
* 1988年，另一个由Xerox, Apple等公司的技术人员组成的小组也开始做类似的事情，他们的工作基于Xerox早期的XCCS编码标准。其中来自Xerox的Joe Becker的论文中首次提到了"**Unicode**"这个词。这个小组也就是今天"Unicode Consortium"的前身。

---
# A Brief History of Unicode (cont.)
* 尽管有着类似的目标，两个团队的技术方案却有着较大的差异。
* ISO 10646的初始版本采用4字节的code point，但对每个字节的部分取值范围做了禁用，其实际code space为：192(groups) x 192(planes) x 192(rows) x 192(cells)。
* 由于采用了4字节的code point，为了节省存储空间而引入了多种复杂的编码方式。
* 引入了"**B**asic **M**ultilingual **P**lane"的概念。
* 将简中、繁中、日文、韩文分配到不同的plane。

---
# A Brief History of Unicode (cont. 2)
* 另一方面，初始版本的Unicode基于2字节的code point设计，大致等价于ISO 10646中的1个plane。
* 对字节的取值范围没有额外限制，因此最大可编码65536个字符。
* 也没有定义其它编码方式，就是每个字符占用2字节。UTF-8之类的编码方式是后面才引入的。
* 对于汉字，Unicode尝试建立一个CJKV中的公共汉字字符子集，并进行统一编码。其思路类似于只分配一个'A'的code point，虽然它同时存在于English, Spanish, French, Italian, German...

---
# A Brief History of Unicode (cont. 3)
* ISO 10646的初始投票没有获得通过~~~
* 两个团队开始讨论技术融合方案：
  - 保留了ISO 10646的32-bit code space，但去掉了单个字节的取值范围限制。
  - 编码方式进行了简化，只保留UCS-4和UCS-2(仅支持BMP)。
  - 采用了Unicode的统一汉字编码方案，并将其放入到BMP中。
* 从1991年开始，两个团队开始做技术方案和码点的统一，使得"Universal Character Set"和"The Unicode Standard"这两个标准在常规使用层面上是等价的，并在后继的演进中维持了同步。

---
# The Unicode Codespace
* 当前最新的Unicode版本是13.0 (March 2020)。
* 共定义了17个平面(planes)，每个平面包含256 x 256个码点(code points)，理论上最大支持的码点(code points)个数为：17 x 65536 = 1,114,112。
* 当前一共给143,859个字符(characters)分配了码点，分属于154种文字(scripts)。

---
# The Unicode Codespace (cont.)
![h:100](codespace-map-legend.png) ![h:500](codespace-map.png)

---
# The Unicode Codespace (cont. 2)
![h:120](script-map-legend.png)
![h:350](script-map.png)

---
# UTF-32
* "UTF" stands for "Unicode Transformation Format".
* Unicode的码点取值范围：U+0000到U+10FFFF。
* UTF-32用4字节整数表示1个code point。
* 需要考虑字节序：UTF-32BE/UTF-32LE。
* 逻辑简单，例如求字符串长度和取字符串中第n个字符都是O(1)的复杂度。
* 由于常用字符都集中在BMP中，单个字符占用4字节是巨大的浪费，实际使用不广泛。

---
# UTF-16
* 初始版本采用2字节整数表示1个code point，因此其表示范围为U+0000到U+FFFF，也即仅能表示BMP中的字符。
* 同样需考虑字节序：UTF-16BE/UTF-16LE。
* 存储效率适中，逻辑简单。微软选择UTF-16作为Windows中Unicode文本的标准编码方式。

---
# UTF-16 (cont.)
* 然而，不能表示BMP之外的字符是个巨大的缺陷，例如生僻汉字、emoji等等。
* 在Unicode 2.0中引入了**surrogates**，也称为surrogate pairs，使得在UTF-16中可以完整表示所有的Unicode code point。
* surrogates通过两个连续的code unit（也即两个连续的uint16），来表示一个值大于U+FFFF的码点。
* 在增强表示能力的同时，它也增加了UTF-16的复杂度。计算字符串长度等操作不再是O(1)复杂度了。

---
# UTF-16 (cont. 2)
* 一个surrogate pair分为high和low两个部分，其二进制形式为110110**xxxxxxxxxx** 110111**yyyyyyyyyy**
* 已知码点的最大值为U+10FFFF。给定一个值大于U+FFFF的码点，将其值减去0x10000，结果的范围是[0, 0xFFFFF]，也即可以用**20个bit**来表示。将这20个bit分为高低两个10bit的部分，分别代入上面的xxxxxxxxxx和yyyyyyyyyy中，就得到了对应的surrogate。

---
# UTF-8
* 由 Ken Thompson 和 Rob Pike 在贝尔实验室的 Plan9 操作系统中首次实现。
* 是一种基于单字节(8 bit)编码单元、可变长度的Unicode字符编码方式。
* 与ASCII直接兼容，在储存英文字符串时空间效率高，是当今Internet和Linux世界的事实字符编码标准。

---
# UTF-8 (cont.)
* 首字节高位为0时，表明此code point用1个字节表示。
* 首字节高位为1时，有几个连续的1就说明此code point用几个连续字节来表示。
* ![h:250](utf-8.png)

---
# UTF-7
[UTF-7](https://en.wikipedia.org/wiki/UTF-7) (7-bit Unicode Transformation Format) is an **obsolete** variable-length character encoding for representing Unicode text using a stream of ASCII characters. It was originally intended to provide a means of encoding Unicode text **for use in Internet E-mail messages** that was more efficient than the combination of UTF-8 with quoted-printable.

---
# BOM
* Byte Order Mark 是一串特定的字节序列，通常放置于字符串的开始处，用来标识后继字符串的字节序。
* UTF-8这种单字节的编码理论上不存在字节序的问题，但为了更方便的进行字符串编码方式的识别，也设计了对应的BOM。
* BOM是可选的，不一定存在。
* ![h:200](bom.png)

---
# Combining character sequence
* 某些文字中会用到变音符号，例如：Café, Jalapeño, TÜV
* ![h:300](diacritical-marks-1.png) → ![h:150](diacritical-marks-2.png)
* Unicode中包含一类被称为"combining marks"的字符，它们可以与基(base)字符进行组合。例如得到一个带变音符号的拉丁字母。

---
# CCS (cont.)
* "Combining marks"字符总是与它前面的字符进行组合。
![](ccs-1.png)
* 当基字符有多个attachable slots时，mark字符的顺序不影响结果。
![](ccs-2.png)

---
# CCS (cont. 2)
* 当多个mark字符attach到同一位置时，其先后顺序对结果有影响。
![](ccs-3.png)

---
# Precomposed characters
* 但是为了**向后兼容**，Unicode也包括了一堆已经预先组合好的字符，直接分配了码点。
![h:300](precomposed.png)
* 感觉刚才这一堆聪明事都白做了...

---
# Unicode equivalence
* **Canonical equivalence** is a fundamental equivalency between characters or sequences of characters which represent the same abstract character, and which when correctly displayed should always have the same visual appearance and behavior.
![h:300](canonical-equivalence.png)

---
# Unicode equivalence (cont.)
* **Compatibility equivalence** is a weaker type of equivalence between characters which represent the same abstract character, but which may have distinct visual appearances or behaviors.
![h:350](compatibility-equivalence.png)

---
# Normalization Forms
* [Unicode Normalization Forms](https://unicode.org/reports/tr15/) are formally defined normalizations of Unicode strings which make it possible to determine whether any two Unicode strings are **equivalent** to each other.
![h:350](normalization-forms-1.png)

---
# Normalization Forms (cont.)
![h:300](normalization-forms-2.png) ![h:450](normalization-forms-3.png)

---
# Normalization Forms (cont. 2)
![h:400](normalization-forms-4.png)

---
# Normalization Forms (cont. 3)
* The [Unicode Normalization Algorithm](https://unicode.org/reports/tr15/#Description_Norm) is fairly complex.
* ![h:400](normalization-forms-5.png)

---
# Some interesting unicode characters
* chess pieces: ♔ ♕ ♖ ♗ ♘ ♙ ♚ ♛ ♜ ♝ ♞ ♟
* playing card suits: ♡ ♢ ♤ ♧ ♥ ♦ ♠ ♣
* mahjong tiles: 🀀 🀁 🀂 🀃 🀅 🀟 🀩
* dice: ⚀ ⚁ ⚂ ⚃ ⚄ ⚅
* weather symbols: ☀ ☁ ☂ ☃ ☄
* musical symbols: ♩ ♪ ♫ ♬ ♭ ♮ ♯

---
<!-- _class: lead -->
# Thanks
