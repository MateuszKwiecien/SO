all: Warehouse Supplier Assembler

Warehouse:
	g++ processes/warehouse.cpp -o warehouse

Supplier:
	g++ processes/supplier.cpp -o supplier

Assembler:
	g++ processes/assembler.cpp -o assembler

clean:
	rm -r warehouse supplier