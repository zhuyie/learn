package main

import (
	"encoding/csv"
	"fmt"
	"io"
	"os"
	"unicode/utf8"
)

type idsEntry struct {
	sequence string
}

func main() {
	file, err := os.Open("./IDS.txt")
	if err != nil {
		fmt.Printf("Open file failed: %v\n", err)
		return
	}
	defer file.Close()

	db := make(map[rune]idsEntry)

	reader := csv.NewReader(file)
	reader.Comma = '\t'
	reader.Comment = '#'
	reader.FieldsPerRecord = -1
	reader.ReuseRecord = true
	var record []string
	for {
		record, err = reader.Read()
		if err != nil {
			break
		}
		if len(record) < 3 {
			continue
		}

		var (
			ch        rune
			n         int
			pos       int
			codepoint rune
			entry     idsEntry
		)

		ch, n = utf8.DecodeRuneInString(record[1])
		if ch == utf8.RuneError || n != len(record[1]) {
			err = fmt.Errorf("invalid column[1]")
			break
		}
		codepoint = ch

		ch, pos = utf8.DecodeRuneInString(record[2])
		if ch != '^' {
			err = fmt.Errorf("invalid column[2]")
			break
		}
		for {
			ch, n = utf8.DecodeRuneInString(record[2][pos:])
			if ch == '$' || ch == utf8.RuneError {
				break
			}
			entry.sequence += string(ch)
			pos += n
		}

		db[codepoint] = entry
	}
	if err != nil && err != io.EOF {
		fmt.Printf("Read file failed: %v\n", err)
		return
	}

	fmt.Printf("Database: IDS.txt\n")
	fmt.Printf("Records : %v\n", len(db))
	fmt.Printf("\n")

	var (
		input string
		ch    rune
	)
	for {
		fmt.Printf("Char: ")
		fmt.Scanf("%s", &input)
		ch, _ = utf8.DecodeRuneInString(input)
		if ch < 0x10000 {
			fmt.Printf("Code: U+%04X\n", ch)
		} else {
			fmt.Printf("Code: U+%06X\n", ch)
		}

		entry, ok := db[ch]
		if !ok {
			fmt.Printf("IDS : not found\n")
			fmt.Printf("\n")
			continue
		}

		fmt.Printf("IDS : %s\n", entry.sequence)
		fmt.Printf("\n")
	}
}
