
SKUDIR = $(INSTALLDIR)/ra_SKU

ifeq ($(BUILD_NAME), RT-AC52U)
SKU_2G_FCC  = "20130903_FCC_2.4GHz_SingleSKU.dat"
SKU_2G_CE   = "20130827_CE_2.4GHz_SingleSKU.dat"
SKU_5G_FCC  = "20130709_FCC_5GHz_RFMD_SingleSKU.dat"
SKU_5G_CE   = "20130709_CE_5GHz_RFMD_SingleSKU.dat"
SKU_2G_SG   = "20130826_SG_2.4GHz_SingleSKU.dat"
SKU_5G_SG   = "20130826_SG_5GHz_RFMD_SingleSKU.dat"
SKU_2G_APAC = 
SKU_5G_APAC = 
endif

SKU_FILES  = $(BUILD_NAME)/$(SKU_2G_FCC) $(BUILD_NAME)/$(SKU_2G_CE) $(BUILD_NAME)/$(SKU_5G_FCC) $(BUILD_NAME)/$(SKU_5G_CE)


# usage: $(call install_file, src_file, dest_file)
define install_file
	@( \
	if [ -e $(1) ]; then \
		install $(1) $(2) ; \
	else \
		echo "File ( $(1) ) not exist" ; exit 1; \
	fi; \
	)
endef



all clean:
	@if [ ! -d $(BUILD_NAME) ]; then \
		echo "No SingleSKU for $(BUILD_NAME)"; \
		exit 1; \
	fi

install: all
	@install -d $(SKUDIR)
	@install $(BUILD_NAME)/$(SKU_2G_FCC) $(SKUDIR)/SingleSKU_FCC.dat
	@install $(BUILD_NAME)/$(SKU_2G_CE)  $(SKUDIR)/SingleSKU_CE.dat
	@install $(BUILD_NAME)/$(SKU_5G_FCC) $(SKUDIR)/SingleSKU_5G_FCC.dat
	@install $(BUILD_NAME)/$(SKU_5G_CE)  $(SKUDIR)/SingleSKU_5G_CE.dat
ifneq ($(SKU_2G_SG),)
	@install $(BUILD_NAME)/$(SKU_2G_SG) $(SKUDIR)/SingleSKU_SG.dat
endif
ifneq ($(SKU_5G_SG),)
	@install $(BUILD_NAME)/$(SKU_5G_SG) $(SKUDIR)/SingleSKU_5G_SG.dat
endif
ifneq ($(SKU_2G_APAC),)
	@install $(BUILD_NAME)/$(SKU_2G_APAC) $(SKUDIR)/SingleSKU_APAC.dat
endif
ifneq ($(SKU_5G_APAC),)
	@install $(BUILD_NAME)/$(SKU_5G_APAC) $(SKUDIR)/SingleSKU_5G_APAC.dat
endif



.PHONY: all clean nstall
.PHONY: $(BUILD_NAME)
