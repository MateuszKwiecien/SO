all: Warehouse Supplier Assembler Director

Warehouse:
	g++ processes/warehouse.cpp IPC_helpers.cpp -o warehouse

Supplier:
	g++ processes/supplier.cpp IPC_helpers.cpp -o supplier

Assembler:
	g++ processes/assembler.cpp IPC_helpers.cpp -o assembler

Director:
	g++ processes/adirector.cpp IPC_helpers.cpp -o director

clean:
	rm -r warehouse supplier assembler file