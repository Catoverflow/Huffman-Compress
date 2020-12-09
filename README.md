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

Dictionary Size: `8bit(char)`

<table>
    <tr>
        <th colspan="7">Header</th>
        <th colspan="2">Data</th>
    </tr>
    <tr>
        <th rowspan="2">alignment</th>
        <th rowspan="2">tree size</th>
        <th colspan="3">huffman node1</th>
        <th rowspan="2">node2</th>
        <th rowspan="2">...</th>
        <th rowspan="2">Huffman bitstream</th>
        <th rowspan="2">align</th>
    </tr>
    <tr>
        <th>huff code length</th>
        <th>huff code</th>
		<th>source code</th>
    </tr>
	<tr>
        <th>8bit</th>
        <th>8bit</th>
        <th>m=8bit</th>
        <th>8mbit<br>m<=17</th>
        <th>8bit</th>
        <th></th>
        <th></th>
        <th>nbit</th>
        <th>8-(n mod8)bit</th>
    </tr>
</table>

## Acknowledgement

First submitted by [Catoverflow](https://github.com/catoverflow), as experiment homework for data structure class  :3
