---
marp: true
theme: gaia
backgroundColor: #fff
---
<style>
section {
  font-family: 'Avenir Next', 'Avenir', 'Trebuchet MS', 'Segoe UI', sans-serif;
}
</style>

<!-- _class: lead -->
# A brief introduction to GPGPU
zhuyie
zhuyie@gmail.com

---
# Agenda
- A demo program
- What is GPU/GPGPU?
- Understanding GPU

---
<!-- paginate: true -->
# Estimate pi using Monte Carlo
* A simple way to estimate the value of pi is from **the area ratio** between a unit quarter-circle and the enclosing unit square. 
![h:250](pi-1.png)![h:60](pi-2.png)

---
# Estimate pi using Monte Carlo
* We can simply select a **large** number of **uniformly distributed** points within the square. The fraction of points **inside** to the **total** number of points is identical to the area ratio between the quarter-circle and the square.
![h:200](pi-3.png)
* For each point, we determine if it is **in** the circle, x^2 + y^2 <= a^2.

---
# Estimate pi using Monte Carlo
![h:500](pi-4.png)

---
# Estimate pi using Monte Carlo
![](pi-5.png)

---
# Estimate pi using Monte Carlo
![h:500](pi-6.png)

---
# What is GPU?

---
# What is GPGPU?
* **G**eneral **P**urpose computation on **G**raphics **P**rocessing **U**nits.

---
# What is GPGPU?
- API
  - **CUDA** - a parallel computing platform and API model created by **Nvidia**. _Nvidia GPU only_.
  - OpenGL/DirectX/Vulkan **Compute Shader**.
  - **OpenCL** - a framework for writing programs that execute across heterogeneous platforms (CPUs, GPUs, DSPs, ...).
  - **Metal** - a low-level, low-overhead hardware-accelerated 3D graphic and compute shader API created by **Apple**.
  - ...

---
# Understanding GPU
- **From a software perspective:**
  - OpenCL have a good abstraction layer.
- **From a hardware perspective:**
  - Closer look at real GPU designs.

---
# OpenCL Platform Model
![h:500](opencl-1.png)

---
# OpenCL Device Example
- ATI Radeon™ HD 5870 GPU
![h:450](opencl-2.png)

---
# OpenCL Device Example
- ATI Radeon™ HD 5870 GPU
![h:450](opencl-3.png)

---
# OpenCL Execution Model
- **Kernel:**
  - Basic unit of executable code that runs on OpenCL devices.
  - Data-parallel or task-parallel.
- **Host program:**
  - Executes on the host system.
  - Sends kernels to execute on OpenCL devices using command queue.

---
# Kernels – Define Work Load
- Define N-dimensional computation domain.
  - N = 1, 2, or 3.
  - Each element in the domain is called a **work-item**.
  - N-D domain (**global dimensions**) defines the total work-items that execute in parallel.
  - Each work-item executes the **same** kernel.
- ![h:100](opencl-4.png)

---
# Kernels - Work-group
- Work-items are grouped into work-groups.
  - **Local dimensions** define the size of the workgroups.
  - Execute together on **same** compute unit.
  - Share **local memory** and **synchronization**.

---
# Kernels - Work-group Example
![h:500](opencl-5.png)

---
# OpenCL C Language
- The programming language that is used to write compute kernels is called **OpenCL C** and is based on C99, but adapted to fit the device model in OpenCL.
![h:300](opencl-6.png)

---
# OpenCL Memory Model
![h:500](opencl-7.png)

---
# OpenCL Memory Model
- **Global** – read and write by all workitems and work-groups.
- **Constant** – read-only by work-items; read and write by host.
- **Local** – used for data sharing; read/write by work-items in same work-group.
- **Private** – only accessible to one work-item.
- Memory management is explicit:
  - Must move data from host to global to local and back.

---
# Demo - Kernel Program
![h:500](demo-1.png)

---
# Demo - Host Program
![h:280](demo-2.png)

---
# Demo - Host Program
![h:360](demo-3.png)
![h:80](demo-4.png)

---
# Demo - Host Program
![h:310](demo-5.png)

---
# Demo - Host Program
![h:310](demo-6.png)

---
# Demo - Host Program
![h:90](demo-7.png)
![h:190](demo-8.png)

---
# Demo - Host Program
![h:500](demo-9.png)

---
# Demo - Output
![h:500](demo-10.png)
