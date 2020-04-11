dir_build		:= build
dir_bin			:= bin

mrp: mrp/Makefile
	@mkdir -p $(dir_bin)
	@mkdir -p $(dir_build)
	@make -f mrp/Makefile

arm: arm/Makefile
	@mkdir -p $(dir_bin)
	@mkdir -p $(dir_build)
	@make -f arm/Makefile


# 清理
clear:
	@rm -rf bin
	@rm -rf build