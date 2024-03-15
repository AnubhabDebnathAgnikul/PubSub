source_1 := application_layer_demo.c
target_1 := application_layer_demo

compiler := gcc 

build:
	@$(compiler) $(source_1) -o $(target_1)
	@echo "Compiled & Built"
assembly:
	@$(compiler) -S $(source_1) -o $(target_1)
	@echo assemblycode generated

clean:
	@if [ -f $(target_1) ]; then\
		rm $(target_1);\
		echo "removed";\
	fi

rebuild:
	@if [ -f $(target_1) ]; then\
		$(MAKE) clean;\
		$(MAKE) build;\
	fi