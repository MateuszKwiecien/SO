all: Warehouse Supplier

Warehouse:
	g++ processes/warehouse.cpp -o warehouse

Supplier:
	g++ processes/supplier.cpp -o supplier

clean:
	rm -r warehouse supplier