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
- GPGPU Programming
- The difference between CPU and GPU
- Closer look at real GPU designs

---
<!-- paginate: true -->
# Estimate pi using Monte Carlo
- A simple way to estimate the value of pi is from **the area ratio** between a unit quarter-circle and the enclosing unit square. 
![h:250](pi-1.png)![h:60](pi-2.png)

---
# Estimate pi using Monte Carlo
- We can simply select a **large** number of **uniformly distributed** points within the square. The fraction of points **inside** to the **total** number of points is identical to the area ratio between the quarter-circle and the square.
![h:200](pi-3.png)
- For each point, we determine if it is **in** the circle, x^2 + y^2 <= a^2.

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
- A [graphics processing unit (GPU)](https://en.wikipedia.org/wiki/Graphics_processing_unit) is a specialized electronic circuit designed to rapidly manipulate and alter memory to accelerate the creation of images in a frame buffer intended for output to a display device. 
- Modern GPUs are very efficient at manipulating computer graphics and image processing. Their **highly parallel structure** makes them more efficient than CPUs for algorithms that process large blocks of data in parallel. 
- The term was popularized by Nvidia in 1999, who marketed the GeForce 256 as "the world's first GPU".

---
# The Graphics Pipeline
![h:500](graphics-pipeline-1.png)

---
# Vertex processing
![h:500](graphics-pipeline-2.png)

---
# Primitive processing
![h:500](graphics-pipeline-3.png)

---
# Rasterization
![h:500](graphics-pipeline-4.png)

---
# Fragment processing
![h:500](graphics-pipeline-5.png)

---
# Pixel operations
![h:500](graphics-pipeline-6.png)

---
# Programmable Graphics Pipeline
![h:500](graphics-pipeline-7.png)

---
# Shader Program Architecture
![h:450](shader-1.png)

---
# Simple OpenGL Vertex Shader
![h:450](shader-2.png)

---
# Simple OpenGL Fragment Shader
![h:500](shader-3.png)

---
# What's in a GPU?
![h:500](graphics-pipeline-8.png)

---
# GPU Computing?
- Beyond basic graphics
  - Collision detection
  - Fluid and cloth
  - Ray tracing
- Beyond graphics
  - Protein folding
  - Speech recognition
  - Fourier transforms

---
# GPGPU
- **G**eneral **P**urpose computation on **G**raphics **P**rocessing **U**nits.
- [Unified shader model](https://en.wikipedia.org/wiki/Unified_shader_model) refers to a form of hardware in a GPU where **all of the shader stages** in the rendering pipeline (geometry, vertex, pixel, etc.) **have the same capabilities**. They can all read textures and buffers, and they use instruction sets that are almost identical.

---
# GPU Computing APIs
- **CUDA** - a parallel computing platform and API model created by **Nvidia**. _Nvidia GPU only_.
- OpenGL/DirectX/Vulkan **Compute Shader**.
- **OpenCL** - a framework for writing programs that execute across heterogeneous platforms (CPUs, GPUs, DSPs, ...).
- **Metal** - a low-level, low-overhead hardware-accelerated 3D graphic and compute shader API created by **Apple**.

---
# GPGPU Programming
- OpenCL have a good abstraction layer.
- Use our estimate-pi program as an example.

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

---
# Latency and Throughput
* **Latency** is a time delay between the moment something is initiated, and the moment one of its effects begins or becomes detectable.
* **Throughput** is the amount of work done in a given amount of time.
* **CPUs** are **low latency low throughput** processors.
* **GPUs** are **high latency high throughput** processors.

---
# Latency and Throughput
* CPUs are designed to **minimize latency**.
  - Example: Mouse or keyboard input.
* CPUs are designed to **maximize single thread performance**.
  - Large caches.
  - Superscalar (execute more than one instruction during a clock cycle).
  - Out-of-order execution.
  - Branch prediction.

---
# Latency and Throughput
* GPUs are designed for tasks that can **tolerate latency**.
  - Example: Graphics in a game (simplified scenario):
  ![h:200](latency-1.png)
* GPUs are designed for tasks that need high throughput, i.e. processing **millions of** pixels in a single frame.

---
# CPU vs GPU Transistor Allocation
* GPUs can have more ALUs for the same sized chip and therefore run many more threads of computation.
![h:250](transistor-allocation.png)
* Modern GPUs run 10,000s of threads concurrently.

---
# CPU die
- Intel Skylake quad-core (14 nm, ~122.3 mm²)
![h:400](die-cpu-1.png) ![h:200](die-cpu-2.png)

---
# GPU die
- NVIDIA GeForce GTX 1080 (16 nm, ~314 mm²)
![h:450](die-gpu-1.jpeg) ![h:450](die-gpu-2.png)
