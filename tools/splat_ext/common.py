from splat.segtypes.common.bss import CommonSegBss


class PSXSegCommon(CommonSegBss):
    def get_linker_section(self) -> str:
        return ".common"
