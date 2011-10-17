
lib/libdel.a: ./include/* ./src/* Makefile
	@echo "=================================="
	@echo " Compiling Triangle++   ..."
	@mkdir -p bin
	@mkdir -p lib
	@./src/compile.py
	@echo -n " Updating Documentation ..."
	@make --quiet doc/html/index.html
	@echo " Done."
	@echo "=================================="

clean:
	@echo Cleaning up temporary files...
	@rm -rf ./doc/* ./bin/*.o ./bin/*.exe ./bin/del.off \
               ./bin/test* ./lib/* ./src/*.tmp &> /dev/null;

doxygen: doc/html/index.html

doc/html/index.html: ./include/* ./src/* 
	doxygen &> /dev/null
