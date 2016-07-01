a.out : btf.c
	cc btf.c
	./a.out < input1.txt
	dot -Tpng opbt.dot> opbt.png
	eog opbt.png
	./a.out < input2.txt
	dot -Tpng opbt.dot> opbt.png
	eog opbt.png
	./a.out < input3.txt
	dot -Tpng opbt.dot> opbt.png
	eog opbt.png
	./a.out < input4.txt
	dot -Tpng opbt.dot> opbt.png
	eog opbt.png
	./a.out < input5.txt
	dot -Tpng opbt.dot> opbt.png
	eog opbt.png
	./a.out < input6.txt
	dot -Tpng opbt.dot> opbt.png
	eog opbt.png
	./a.out < input7.txt
	dot -Tpng opbt.dot> opbt.png
	eog opbt.png
	./a.out < input8.txt
	dot -Tpng opbt.dot> opbt.png
	eog opbt.png


compile:
	cc -g btf.c
	gdb ./a.out

input:
	cc createinput.c
	./a.out
