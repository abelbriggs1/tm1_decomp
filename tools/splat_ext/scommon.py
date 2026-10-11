from pathlib import Path

from splat.segtypes.common.sbss import CommonSegSbss
from splat.segtypes.linker_entry import LinkerEntry, LinkerWriter


class ScommonLinkerEntry(LinkerEntry):
    """
    Places every small common symbol from every object.

    There is no object to build for this entry; `ld` allocates common symbols
    itself, grouped by the first object that defines them and then ordered by
    a hash of their names.
    """

    def __init__(self, segment: "PSXSegScommon"):
        super().__init__(
            segment,
            [],
            Path(segment.name),
            segment.get_linker_section_order(),
            segment.get_linker_section_linksection(),
            segment.is_noload(),
        )
        self.object_path = None

    def emit_path(self, linker_writer: LinkerWriter):
        linker_writer._write_object_path_section(Path("*"), ".scommon")


class PSXSegScommon(CommonSegSbss):
    def get_linker_section(self) -> str:
        return ".scommon"

    def should_self_split(self) -> bool:
        return False

    def get_linker_entries(self) -> list[LinkerEntry]:
        return [ScommonLinkerEntry(self)]
