# Distribution licenses

Daze's own source is MIT licensed; see `LICENSE.md`. The distributed executable
includes Unicorn and is distributed as a combined work under GPLv2. It comes
without warranty. Dependency license texts accompany the executable as separate
release assets and are exported locally in `dist/licenses/`.
Daze's source is the release's published source archive; Unicorn and glibc
sources are identified below.

| Component | License and notices | Corresponding source |
| --- | --- | --- |
| Unicorn 2.1.4, including its QEMU-derived code | GPLv2; additional per-file notices are preserved in the source | Upstream Unicorn 2.1.4 at the exact revision recorded in `BUILD-INFO.txt` |
| GNU C Library (glibc), statically linked | LGPL-2.1-or-later and per-file licenses; see the `glibc-copyright` asset | Ubuntu source package for the version recorded in `BUILD-INFO.txt`, including distribution patches |
| GCC runtime (libgcc), statically linked | GPLv3 with the GCC Runtime Library Exception; see the `gcc-copyright` asset | Covered by the Runtime Library Exception for this GCC build |

Exact dependency/compiler versions and the Unicorn revision are in
`BUILD-INFO.txt`. Upstream copyright and license notices are retained in
the accompanying license assets. Daze and Unicorn can be rebuilt and
relinked with a modified glibc; see `docs/RELEASING.md`.

The container includes notices under `/usr/share/daze/`.
DOS applications and test fixtures are separate works and are
not included in the release or covered by Daze's MIT license.
