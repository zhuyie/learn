---
marp: true
theme: gaia
backgroundColor: #FFFFFF
color: #444444
---
<style>
h1 {
  font-size: 1.6em;
  color: #1B558E;
}
section {
  font-family: 'Avenir Next', 'Avenir', 'Trebuchet MS', 'Segoe UI', sans-serif;
  padding: 50px;
}
</style>

<!-- _class: lead -->
# Meltdown && Spectre
### 从硬件漏洞看现代CPU架构
zhuyie
zhuyie@gmail.com

---
<!-- paginate: true -->
# Agenda
- todo

---
# Mitigations
* [Spectre mitigations in MSVC](https://devblogs.microsoft.com/cppblog/spectre-mitigations-in-msvc/)
* [Understanding the performance impact of Spectre and Meltdown mitigations on Windows Systems](https://www.microsoft.com/security/blog/2018/01/09/understanding-the-performance-impact-of-spectre-and-meltdown-mitigations-on-windows-systems/)
* Spectre V1 (Bounds Check Bypass)
  - **Compiler** change; Recompiled **system binaries** distributed by Windows Updates.
  - Edge & IE11 **hardened** to prevent exploit from JavaScript.
* Meltdown (Rogue Data Cache Load)
  - **Isolate** kernel and user mode **page tables**.

---
# Mitigations - Performance
![h:500](mitigations-performance.png)

---
# Mitigations - MSVC /Qspectre
* For Intel and similar processors the **LFENCE** instruction will stop younger instructions from executing, even speculatively, before older instructions have retired. 
* An LFENCE instruction inserted **after a bounds check** will **prevent** younger operations from executing before the bound check retires. Therefore it can act as a speculation barrier.
* The MSVC compiler has been updated with support for the /Qspectre switch which will **automatically** insert one of these **speculation barriers** when the compiler **detects** instances of variant 1. 

---
# Mitigations - MSVC /Qspectre
![h:500](MSVC-Qspectre-1.png)

---
# Mitigations - MSVC /Qspectre
![h:500](MSVC-Qspectre-2.png)

---
# Mitigations - MSVC /Qspectre
![h:500](MSVC-Qspectre-3.png)

---
# Mitigations - MSVC /Qspectre
![h:500](MSVC-Qspectre-4.png)

---
# Mitigations - KPTI
* [The current state of kernel page-table isolation](https://lwn.net/Articles/741878/)
* In old kernels, each process has a **single** page global directory (PGD). one of the first steps taken in the KPTI is to create a **second** PGD.
* The original remains in use when the kernel is running; it maps the full address space. The second is made active when the process is running in **user space**. It points to the same directory hierarchy for pages belonging to the process itself, but the portion describing kernel space is **mostly absent**.
* **Switch** page table when enter/exit kernel space.

---
# Mitigations - KPTI
* KPTI will affect performance for anything that does **system calls** or **interrupts**. Just the new instructions (CR3 manipulation) add a few hundred cycles to a syscall or interrupt.
* Old CPUs (without PCIDs) require a **TLB flush** when switching the page table, which hurt performance badly.
* For database engine PostgreSQL the impact on read-only tests on an Intel Skylake processor was 7–17% (or 16–23% without PCIDs). [link](https://www.postgresql.org/message-id/20180102222354.qikjmf7dvnjgbkxe%40alap3.anarazel.de)
* Intel **hardware fixes** to Meltdown from Cascade Lake. [link](https://www.extremetech.com/computing/275776-intel-discusses-cascade-lakes-improvements-hardware-mitigations-for-meltdown-spectre)

---
# References
- https://newsroom.intel.com/wp-content/uploads/sites/11/2018/01/Intel-Analysis-of-Speculative-Execution-Side-Channels.pdf
- https://developer.arm.com/-/media/Files/pdf/Cache_Speculation_Side-channels.pdf
- https://www.raspberrypi.com/news/why-raspberry-pi-isnt-vulnerable-to-spectre-or-meltdown/
- https://llvm.org/docs/SpeculativeLoadHardening.html
