savedcmd_drivers/iommu/built-in.a := rm -f drivers/iommu/built-in.a;  printf "drivers/iommu/%s " arm/built-in.a iommufd/built-in.a | xargs ar cDPrST drivers/iommu/built-in.a
