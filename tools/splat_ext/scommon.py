from splat.segtypes.common.bss import CommonSegBss


class PSXSegScommon(CommonSegBss):
    def get_linker_section(self) -> str:
        return ".scommon"
