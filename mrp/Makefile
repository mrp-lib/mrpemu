# 目录定义
build_dir 		:=		build/mrp
src_dir 		:= 		mrp/src
c_includes		:=		mrp/include
target			:=		bin/mrp

# 编译器定义
cc				:=		arm-linux-gnueabi-gcc

c_flags			:=		-I $(c_includes) \
						-g

# 输入文件扫描
files := $(shell find $(src_dir) -name '*.c')
c_files := $(files:$(src_dir)/%=%)


# 输入文件和中间文件
input_files 	:=		$(addprefix $(src_dir)/, $(c_files))
build_files 	:=		$(addprefix $(build_dir)/, $(c_files:.c=.o))
build_dirs		:=		$(dir $(build_files))

# 连接得到目标
$(target): $(build_files)
	@mkdir -p $(dir $(target))
	@echo "[LK] -> $@"
	@$(cc) -lz -o $@ $^ -lpthread --static
	@echo "complate!"

# 逐个编译文件
$(build_files): $(build_dir)/%.o : $(src_dir)/%.c
	@mkdir -p $(dir $@)
	@echo "[CC] $< -> $@"
	@$(cc) -c $(c_flags) -o $@ $<

# 清理
clear:
	@rm -f $(target)
	@rm -rf $(build_dir)

