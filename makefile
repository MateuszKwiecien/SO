all: Warehouse Supplier Assembler

Warehouse:
	g++ processes/warehouse.cpp IPC_helpers.cpp -o warehouse

Supplier:
	g++ processes/supplier.cpp IPC_helpers.cpp -o supplier

Assembler:
	g++ processes/assembler.cpp IPC_helpers.cpp -o assembler

clean:
	rm -r warehouse supplier