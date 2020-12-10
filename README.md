# Huffman Compression

## Usage

`huffman [options] source [target]`

Options:

​	`-u` uncompress file

​	`-z` compress file (by default)

​	`-r` rename output (should be used with target specified, `${source}.huff` by default)

Examples:

`huffman -u zip.huff`

`huffman -zr code.txt code.huff`

## Details

### `.huff` File Formatting

__Dictionary Size__

`8bit(char)`

__File Structure__:

<table>
    <th style="text-align:center">Header</th>
    <th style="text-align:center">Data</th>
</table>


__Header Structure__:

<table>
    <tr>
        <th colspan="11" style="text-align:center">Header</th>
    </tr>
    <tr>
        <th rowspan="2">tree size</th>
        <th colspan="7">node 1</th>
        <th rowspan="2">node 2</th>
        <th rowspan="2">...</th>
        <th rowspan="2">node n</th>
    </tr>
    <tr>
        <th>huff code length</th>
        <th>huff code block 1</th>
        <th>huff code block 2</th>
        <th>...</th>
        <th>margin part for last block</th>
        <th>huff code block (m+7)/8</th>
		<th>source code</th>
    </tr>
	<tr>
        <th>n(8bit)</th>
        <th>m(8bit)</th>
        <th>8bit</th>
        <th>8bit</th>
        <th></th>
        <th>-m mod8 bit</th>
        <th>m mod8 bit</th>
        <th>8bit</th>
        <th></th>
        <th></th>
        <th></th>
    </tr>
</table>


__Data Structure__:

<table>
    <tr>
    	<th colspan="3" style="text-align:center">Data</th>
    </tr>
    <tr>
        <th>huffman bitstream</th>
        <th>alignment</th>
        <th>alignment size</th>
    </tr>
    <tr>
        <th></th>
        <th>a bit</th>
        <th>a(8bit)</th>
    </tr>
</table>



## Acknowledgement

First submitted by [Catoverflow](https://github.com/catoverflow), as experiment homework for data structure class  :3
