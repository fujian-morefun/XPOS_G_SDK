

#库路径
LIB_PATH=	$(OUTPUT_PATH) \
			$(LINUX_PLAT_COMPONENT_LINUX)/platform/libec20 \
			$(QL_EXP_LDLIBS) \
		
STD_LIB=dsi_netctrl dsutils qmiservices qmi_cci \
         qmi_common_so qmi mcm ql_mgmt_client

QL_LIB=ql_at ql_common_api ql_peripheral ql_qcmap_client m

		
#链接库
APP_LIB=apppub atc vcps rupay quics paypass dpas amex emv emvpub entry emvapi api_xpos net power producttest pub rpc switchcheck tms ucosii xgui \
    pthread mf_driver mbedtls wifi json \


#    stdc++ gcc_s c usb sgfpamx sgfdu05 sgfplib 

    
LINK_LIB= $(APP_LIB) $(QL_LIB) $(STD_LIB)

#include目录路径
INC_PATH=$(LINUX_PLAT_COMPONENT_LINUX) \
			$(LINUX_PLAT_COMPONENT_LINUX)/EntryPoint/lib_emvpub/inc \
			$(LINUX_PLAT_COMPONENT_LINUX)/platform/inc \
			$(PROJECT_PATH)/src \
			$(PROJECT_PATH)/../uninpayui/src \
			$(PROJECT_PATH)/../uninpayui/src/page \
			$(LINUX_PLAT_COMPONENT_LINUX)/apppub/inc \


#source文件列表urce文件列表
SRC_DIRS := $(shell find $(PROJECT_PATH)/src -maxdepth 5 -type d)
SRC_LIST := $(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.c)) 
