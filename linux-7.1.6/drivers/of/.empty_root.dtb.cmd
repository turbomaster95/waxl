savedcmd_drivers/of/empty_root.dtb := gcc -E -Wp,-MMD,drivers/of/.empty_root.dtb.d.pre.tmp -nostdinc -I ./scripts/dtc/include-prefixes -undef -D__DTS__ -x assembler-with-cpp -o drivers/of/.empty_root.dtb.dts.tmp drivers/of/empty_root.dts ; ./scripts/dtc/dtc -o drivers/of/empty_root.dtb -b 0 -idrivers/of/ -i./scripts/dtc/include-prefixes -Wno-unique_unit_address -Wno-unit_address_vs_reg -Wno-avoid_unnecessary_addr_size -Wno-alias_paths -Wno-interrupt_map -Wno-simple_bus_reg   -d drivers/of/.empty_root.dtb.d.dtc.tmp drivers/of/.empty_root.dtb.dts.tmp ; cat drivers/of/.empty_root.dtb.d.pre.tmp drivers/of/.empty_root.dtb.d.dtc.tmp > drivers/of/.empty_root.dtb.d 

source_drivers/of/empty_root.dtb := drivers/of/empty_root.dts

deps_drivers/of/empty_root.dtb := \

drivers/of/empty_root.dtb: $(deps_drivers/of/empty_root.dtb)

$(deps_drivers/of/empty_root.dtb):
