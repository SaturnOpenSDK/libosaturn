PROJECTS:= \
	libyaul \
	libbcl \
	libfixmath \
	libtga

ifeq ($(strip $(INSTALL_ROOT)),)
  $(error Undefined INSTALL_ROOT (install root directory))
endif

ifeq ($(strip $(BUILD_ROOT)),)
  $(error Undefined BUILD_ROOT (build root directory))
endif

ifeq ($(strip $(BUILD)),)
  $(error Undefined BUILD (build directory))
endif

SILENT?= @
V_BEGIN_BLACK= [1;30m
V_BEGIN_RED= [1;31m
V_BEGIN_GREEN= [1;32m
V_BEGIN_YELLOW= [1;33m
V_BEGIN_BLUE= [1;34m
V_BEGIN_MAGENTA= [1;35m
V_BEGIN_CYAN= [1;36m
V_BEGIN_WHITE= [1;37m
V_END= [m

define macro-generate-build-rule
$1-$2:
	$(SILENT)mkdir -p $(BUILD_ROOT)/$(BUILD)
	$(SILENT)printf -- "$(V_BEGIN_CYAN)$1$(V_END) $(V_BEGIN_GREEN)$1$(V_END)\n"
	$(SILENT)($(MAKE) BUILD=$(BUILD) -C $1 $2) || exit $$?
endef

define macro-generate-install-rule
$1-$2:
	$(SILENT)mkdir -p $(INSTALL_ROOT)/lib
	$(SILENT)mkdir -p $(INSTALL_ROOT)/include
	$(SILENT)printf -- "$(V_BEGIN_CYAN)$1$(V_END) $(V_BEGIN_GREEN)$1$(V_END)\n"
	$(SILENT)($(MAKE) BUILD=$(BUILD) -C $1 $2) || exit $$?
endef

define macro-generate-clean-rule
$1-$2:
	$(SILENT)printf -- "$(V_BEGIN_CYAN)$1$(V_END) $(V_BEGIN_GREEN)$1$(V_END)\n"
	$(SILENT)($(MAKE) BUILD=$(BUILD) -C $1 $2) || exit $$?
endef

define macro-install
	$(SILENT)mkdir -p $(INSTALL_ROOT)/lib
	$(SILENT)mkdir -p $(INSTALL_ROOT)/include
	$(SILENT)for project in $(PROJECTS); do \
		printf -- "$(V_BEGIN_CYAN)$@$(V_END) $(V_BEGIN_GREEN)$$project$(V_END)\n"; \
		($(MAKE) BUILD=$(BUILD) -C $$project $@) || exit $$?; \
	done
endef

.PHONY: all release release-internal debug install install-release install-release-internal install-debug clean clean-release clean-release-internal clean-debug examples clean-examples tools install-tools clean-tools list-targets

all: release release-internal debug tools

$(foreach project,$(PROJECTS),$(eval $(call macro-generate-build-rule,$(project),release)))
$(foreach project,$(PROJECTS),$(eval $(call macro-generate-build-rule,$(project),release-internal)))
$(foreach project,$(PROJECTS),$(eval $(call macro-generate-build-rule,$(project),debug)))

release release-internal debug:
	$(SILENT)mkdir -p $(BUILD_ROOT)/$(BUILD)
	$(SILENT)for project in $(PROJECTS); do \
		printf -- "$(V_BEGIN_CYAN)$@$(V_END) $(V_BEGIN_GREEN)$$project$(V_END)\n"; \
		($(MAKE) BUILD=$(BUILD) -C $$project $@) || exit $$?; \
	done

install: install-release install-tools

install-release: release
	$(call macro-install)

install-release-internal: release-internal
	$(call macro-install)

install-debug: debug
	$(call macro-install)

$(foreach project,$(PROJECTS),$(eval $(call macro-generate-install-rule,$(project),install-release)))
$(foreach project,$(PROJECTS),$(eval $(call macro-generate-install-rule,$(project),install-release-internal)))
$(foreach project,$(PROJECTS),$(eval $(call macro-generate-install-rule,$(project),install-debug)))

clean: clean-release clean-tools

distclean: clean-release clean-release-internal clean-debug clean-tools clean-examples
	$(SILENT)$(RM) -r $(BUILD_ROOT)/$(BUILD)

clean-release clean-release-internal clean-debug:
	$(SILENT)for project in $(PROJECTS); do \
		printf -- "$(V_BEGIN_CYAN)$@$(V_END) $(V_BEGIN_GREEN)$$project$(V_END)\n"; \
		($(MAKE) BUILD=$(BUILD) -C $$project $@) || exit $$?; \
	done

$(foreach project,$(PROJECTS),$(eval $(call macro-generate-clean-rule,$(project),clean)))

$(foreach project,$(PROJECTS),$(eval $(call macro-generate-clean-rule,$(project),clean-release)))
$(foreach project,$(PROJECTS),$(eval $(call macro-generate-clean-rule,$(project),clean-release-internal)))
$(foreach project,$(PROJECTS),$(eval $(call macro-generate-clean-rule,$(project),clean-debug)))

examples:
	$(SILENT)($(MAKE) -C examples all) || exit $$?

clean-examples:
	$(SILENT)($(MAKE) -C examples clean) || exit $$?

tools:
	$(SILENT)($(MAKE) -C tools all) || exit $$?

install-tools: tools
	$(SILENT)($(MAKE) -C tools install) || exit $$?

clean-tools:
	$(SILENT)($(MAKE) -C tools clean) || exit $$?

list-targets:
	@$(MAKE) -pRrq -f $(firstword $(MAKEFILE_LIST)) : 2>/dev/null | \
	awk -v RS= -F: '/^# File/,/^# Finished Make data base/ {if ($$1 !~ "^[#.]") {print $$1}}' | \
	sort | \
	grep -E -v -e '^[^[:alnum:]]' -e '^$@$$'
