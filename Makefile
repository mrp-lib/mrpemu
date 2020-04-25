include make/Makefile.cfg

# 链接库列表
LIBS			:= libiconv
# 要单独编译的文件，这些文件相对于SRC_DIR目录
FILES			:=	
# 中间代码，这些文件位于BUILD_DIR目录下，并且名称和Makefile后缀名称一致
OBJ_FILES		:=	zlib.o arm.o mrp.o utils.o helper.o

# 生成动态链接库是不能有main函数
ifneq ($(SHARED), TRUE)
	FILES		+=	main.c
endif

# 编译选项扩展，当有需要时添加对应的选项
CC_FLAGS_EX		:=	$(CC_FLAGS)

# 相关路径的处理
MAKE_OBJ_FILES	:=	$(addprefix $(BUILD_DIR)/, $(OBJ_FILES))
C_OBJ_FILES		:=	$(addprefix $(BUILD_DIR)/, $(FILES:%.c=%.o))

# 构建最终目标
$(TARGET_BIN): $(C_OBJ_FILES) $(MAKE_OBJ_FILES)
	@mkdir -p $(dir $@)
	@echo "[CC] $^ -> $@"
	@$(CC) $(LINK_FLAGS) -o $@ $^ -lm

# 编译需要单独编译的c文件
$(C_OBJ_FILES): $(BUILD_DIR)/%.o : $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	@echo "[CC] $< -> $@"
	@$(CC) -c $(CC_FLAGS_EX) -o $@ $<

# 对其他的中间代码进行构建
$(MAKE_OBJ_FILES): $(BUILD_DIR)/%.o: ./make/Makefile.%
	@mkdir -p $(dir $@)
	@echo [MK] "$^ -> $@"
	@make -f $^ TARGET=$@ --no-print-directory

clear:
	@rm -rf $(TARGET_BIN)
	@rm -rf $(BUILD_DIR)
