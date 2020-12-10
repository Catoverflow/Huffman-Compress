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
        <th rowspan="2">alignment size</th>
        <th rowspan="2">tree size</th>
        <th colspan="3">huffman node1</th>
        <th rowspan="2">node2</th>
        <th rowspan="2">...</th>
        <th rowspan="2">huffman bitstream</th>
        <th rowspan="2">alignment</th>
    </tr>
    <tr>
        <th>huff code length</th>
        <th>huff code</th>
		<th>source code</th>
    </tr>
	<tr>
        <th>a=8bit</th>
        <th>8bit</th>
        <th>m=8bit</th>
        <th>8m bit</th>
        <th>8bit</th>
        <th></th>
        <th></th>
        <th>nbit</th>
        <th>a bit</th>
    </tr>
</table>

## Acknowledgement

First submitted by [Catoverflow](https://github.com/catoverflow), as experiment homework for data structure class  :3

