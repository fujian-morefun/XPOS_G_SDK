#头文件,库文件路径,引用库
include $(PROJECT_PATH)/build/source.mak

INC_PATH:=$(addprefix -I, $(INC_PATH))
LIB_PATH:=$(addprefix -L, $(LIB_PATH))
LINK_LIB:=$(addprefix -l, $(LINK_LIB))
OBJ_LIST:=$(addprefix $(OBJECT_PATH)/, $(addsuffix .o, $(basename $(SRC_LIST))))
DEP_LIST:=$(addprefix $(DEPEND_PATH)/, $(addsuffix .d, $(basename $(SRC_LIST))))

#编译及链接参数
CFLAGS:=$(CFLAGS) $(INC_PATH)
CXXFLAGS:=$(CXXFLAGS) $(INC_PATH)
LDFLAGS:=$(LDFLAGS) $(LIB_PATH) $(LINK_LIB) $(LINK_STATIC_LIB) $(TARGET_TYPE)

#指明默认目标是生成最终可执行文件。
all: $(TARGET_IMAGE) 

$(TARGET_IMAGE): $(OBJ_LIST)
	@mkdir -p $(dir $@)
	@echo Link program file: $@
#	$(CAR) $(LDFLAGS) $@ $(filter %.o ,$+)
	$(CC) $(LDFLAGS) -o $@ $(filter %.o ,$+)
	@echo Build Complete!

#生成依赖关系文件
$(addprefix $(DEPEND_PATH)/, %.d):%.c
	@echo Generate depend file $@
	@mkdir -p $(dir $@)
	@$(CC) -MM $(CFLAGS) $< > $@.$$$$; \
	sed 's%^\s*\(.*\.o:\s*\)%$(OBJECT_PATH)/$(dir $<)\1%g' $@.$$$$ > $@; \
	rm -f $@.$$$$

$(addprefix $(DEPEND_PATH)/, %.d):%.cpp
	@echo Generate depend file $@
	@mkdir -p $(dir $@)
	@$(CXX) -MM $(CXXFLAGS) $< > $@.$$$$; \
	sed 's%^\s*\(.*\.o:\s*\)%$(OBJECT_PATH)/$(dir $<)\1%g' $@.$$$$ > $@; \
	rm -f $@.$$$$

#生成OBJ文件
$(addprefix $(OBJECT_PATH)/, %.o):%.c
	@echo Compiler source file: $<
	@mkdir -p $(dir $@)
	$(CC) -c $(CFLAGS) $< -o $@

$(addprefix $(OBJECT_PATH)/, %.o):%.cpp
	@echo Compiler source file: $<
	@mkdir -p $(dir $@)
	$(CXX) -c $(CXXFLAGS) $< -o $@

# 包含入依赖关系文件
-include $(DEP_LIST)

# 删除生成的中间文件
clean:
	-rm -r $(OBJECT_PATH)
	-rm -r $(DEPEND_PATH)
#	-rm -r $(TARGET_PATH)
	-rm $(TARGET_IMAGE)

disenv:
	@echo "LINUX_PLAT_COMPONENT_LINUX = $(LINUX_PLAT_COMPONENT_LINUX)"
	@echo "PROJECT_PATH = $(PROJECT_PATH)"
	@echo "DEV_LCD = $(DEV_LCD)"
	@echo "OUTPUT_PATH = $(OUTPUT_PATH)"
	@echo "C_DEV_LCD = $(C_DEV_LCD)"
	@echo "TARGET_IMAGE = $(TARGET_IMAGE)"
	@echo "CFLAGS = $(CFLAGS)"
	@echo "CFLAGS = $(CFLAGS)"
	@echo "CXXFLAGS = $(CXXFLAGS)"
	