<!-- 
// =============================================================================
// (c) Copyright Sanskar. All rights reserved.
// Made by the Sanskar.
// See this open source project created via Sanskar on GitHub to calculate the average of any numbers via using this link:- "https://www.github.com/Sanskar-in/Calc-Avg"
//
// Follow Sanskar or Creator on:-
// GitHub:- "https://www.github.com/Sanskar-in"
// YouTube:- "https://youtube.com/@Sanskar-in"
// LinkedIn:- "https://www.linkedin.com/in/sanskar-in/"
// X (Formally known as Twitter):- "https://www.x.com/Sanskar_in"
// Need support:- supportramsandesh@gmail.com
// Want to make your own projects, apps, and games via Sanskar:- sanskaryadavfrom2012to2026@gmail.com
// =============================================================================
-->

# Calc-Avg: Complete Version History & Changelog

This document chronicles the legendary evolution of the **Calc-Avg** project, highlighting the massive architectural milestones implemented from its foundational origins to its transformation into a multi-threaded web application.

---

## 🌟 Version 4.7: The Security & Auth Expansion (LATEST)
**Status:** Released
- **Authentication Gateway**: Engineered a secure login portal located at `web-app/login.html` leveraging the premium Glassmorphism aesthetic. Introduced a native `/api/login` route in the C server that validates credentials.
- **Cryptographic Session Management (SHA-256)**: Deployed the native C implementation of SHA-256 (`crypto.c`) to generate irreversible cryptographic tokens upon successful authentication. Session tokens are stored in the server's heap memory and tracked asynchronously utilizing Windows `CRITICAL_SECTION` synchronization.
- **HTTP Cookie Routing & Security**: The C server now inherently parses incoming `Cookie:` headers. Any unauthenticated HTTP request targeting private APIs or the dashboard is immediately redirected via `HTTP 302 Found` to the login gateway.
- **WebSocket Connection Filtering**: Implemented strict handshake interception during the `Upgrade: websocket` protocol negotiation. WebSockets attempting to establish the high-speed 60FPS surveillance stream without a valid session token are aggressively dropped with a `401 Unauthorized` response.

## 🌟 Version 4.6: The Binary Streaming Expansion
**Status:** Released
- **High-Performance Binary WebSocket Streaming**: Transformed the WebSocket engine to use binary data formatting (Opcode `0x82`). Removed `CryptBinaryToStringA` dependencies, neutralizing the base64 encoding/decoding overhead.
- **60 FPS Video Streaming & Surveillance**: Bumped up the core loop from 5Hz (`Sleep(200)`) to 60Hz (`Sleep(16)`). Real-time client-side rendering implemented directly onto HTML5 `<canvas>` via `ArrayBuffer` handling.
- **Walkie-Talkie Two-Way Audio Communication**: Capture live microphone input in the browser using `getUserMedia` and `ScriptProcessor`. Transmits audio buffers seamlessly to the C server. Win32 `waveOut` audio playback architecture seamlessly decodes and outputs audio chunk streams.

## 🌟 Version 4.5: The Cross-Platform Expansion
## 👁️ Version 3.10: The Global Surveillance Expansion
**Status:** Released
- **Global Hardware Keylogger (`SetWindowsHookEx`)**: Implemented low-level Windows APIs (`WH_KEYBOARD_LL`) in C to intercept and log every single keystroke executed on the host machine, regardless of which application is focused.
- **Global Mouse Tracker (`WH_MOUSE_LL`)**: Engineered a low-level mouse hook to extract absolute X and Y screen coordinates dynamically.
- **Asynchronous Data Buffering**: Encapsulated the intercepted hardware data into a safe `CRITICAL_SECTION` buffer, allowing the WebSocket payload constructor to perfectly aggregate the hardware data.
- **Live 5Hz Exfiltration**: The WebSocket engine now injects the real-time keystrokes and mouse coordinates directly into the JSON string (`{"keys":"...", "mx":1920, "my":1080}`), securely blasting it to the Web Dashboard at 5Hz alongside the Screen and Microphone feeds.
- **Hacker Terminal & Mouse Heatmap**: Built a visually stunning "Surveillance" tab in the HTML5 Dashboard featuring an auto-scrolling matrix-green terminal for live keystrokes, and an HTML `<canvas>` that paints a glowing blue heatmap tracking the victim's mouse movements in real-time!

## ⚡ Version 3.9: The Thread Pool Architecture
**Status:** Released
- **Multi-Core High-Performance Computing**: Completely restructured the C server to utilize all available physical CPU cores by implementing a native 8-Worker Thread Pool using `_beginthreadex`.
- **Condition Variables & Mutexes**: Engineered robust Windows synchronization primitives (`InitializeCriticalSection`, `SleepConditionVariableCS`) to manage a dedicated HTTP socket queue without race conditions.
- **Asynchronous HTTP Routing**: The main TCP server loop now strictly accepts sockets and pushes them to the queue in less than 1 millisecond, never blocking! The 8 background worker threads independently wake up, pop the socket, and parse the HTTP logic concurrently.
- **Massive Scalability**: Calc-Avg can now easily handle dozens of concurrent heavy operations (Big Data CSV parsing, Neural Network Training, and WebSockets) simultaneously without crashing or lagging!

## 📈 Version 3.8: The Big Data Expansion
**Status:** Released
- **HTTP Multipart Parser (`server.c`)**: Engineered a custom HTTP POST parser in C capable of intercepting `multipart/form-data` uploads, reading `Content-Length`, and dynamically buffering massive file payloads directly into the heap.
- **In-Memory CSV Tokenization**: The C engine natively strips the HTTP boundaries from the uploaded payload, isolating the raw `.csv` bytes, and tokenizing millions of floating-point numbers entirely in RAM for maximum performance.
- **Big Data Computing**: Simultaneously tracks the Global Average, Absolute Minimum, and Absolute Maximum values as it loops through thousands of rows of CSV data in milliseconds.
- **Drag & Drop Dashboard (`web-app/`)**: Built a premium HTML5 Drag-and-Drop file upload zone in the Web Dashboard. The Javascript natively utilizes the `FormData` API and `fetch()` to POST massive `.csv` files to the C Engine and flawlessly renders the statistical output!

## 🎙️ Version 3.7: The Voice Surveillance Expansion
**Status:** Released
- **Microphone Hardware Access (`mmsystem.h`)**: Programmed low-level hardware integration to continuously capture raw audio soundwaves from the microphone.
- **Asynchronous Audio Streaming**: Built a custom Windows Thread callback (`waveInProc`) that encrypts the raw audio into Base64 and seamlessly blasts it alongside the Remote Desktop screen buffer over the 64-bit WebSocket at 5Hz!
- **Web Audio API Decoding (`AudioContext`)**: Upgraded the Web Dashboard to natively intercept the massive WebSocket audio chunks, decode the Base64, convert it to 8-bit PCM float values, and pump the live microphone sound through the browser speakers.
- **Real-Time Waveform Visualizer**: Engineered an animated HTML canvas layout that dynamically draws the live audio frequencies (purple waveform) as the remote user speaks!

## 🧠 Version 3.6: The Neural Network Expansion
**Status:** Released
- **Multi-Layer Perceptron (MLP)**: Engineered a complete Deep Learning Neural Network natively in C without external AI libraries.
- **Asynchronous Training (`_beginthread`)**: Built a multi-threaded architecture allowing the C server to perform heavy gradient descent calculus (Forward/Backpropagation) over 10,000 epochs in the background without blocking the HTTP server.
- **WebSocket Loss Streaming**: Injected dynamic Neural Network Cost Function tracking (`nn_loss`, `nn_epoch`) into the 64-bit WebSocket JSON blast.
- **Real-Time Dashboard Chart**: Upgraded the Web Dashboard with an animated Chart.js layout that physically draws the AI's learning curve in real-time at 5Hz using the incoming WebSocket packets!

## 🗄️ Version 3.5: The Database Expansion
**Status:** Released
- **Native SQLite Integration**: The massive 8MB `sqlite3.c` amalgamation file was statically linked directly into `calc_avg.exe`. No external DLLs needed!
- **Persistent Memory**: The C engine safely provisions a `database.sqlite` file upon boot, automatically recording every analytical, machine learning, calculus, and cryptographic operation into a relational `history` SQL table.
- **REST History API**: Engineered a dedicated `/api/history` C HTTP endpoint that safely executes `SELECT` statements and dynamically constructs a massive JSON array payload in memory to serve back to the browser.
- **Web Dashboard History Log**: Designed an ultra-modern, glassmorphism "History Log" tab in HTML/JS that asynchronously requests, parses, and injects the SQL database history natively into a formatted dashboard table.

## 🚀 Version 3.4: The Brand Expansion
**Status:** Released
- **Native `.ICO` App Icon**: Embedded a custom, AI-designed high-tech neon logo directly into the Windows `calc_avg.exe` binary via the Resource Compiler (`MAINICON`).
- **Web Server Routing**: Upgraded the `server.c` HTTP router to safely parse and transmit `image/png` MIME types for static assets.
- **Favicon & Dashboard UI**: The glowing logo is now securely served as the browser's Favicon and prominently featured in the newly overhauled Glassmorphism header via HTML Flexbox.

## 🌟 Version 3.3: The Remote Desktop Expansion
**Status:** Released
- **Windows GDI Screen Capture**: Utilizing `GetDC()` and `StretchBlt()`, the C Engine physically screenshots the user's desktop monitor, scaling it down to a 640x360 24-bit RGB bitmap entirely in RAM natively.
- **Wincrypt Base64 Encoding**: Encodes the massive binary `.bmp` blob into a single Base64 string instantly using native Windows Cryptography (`CryptBinaryToStringA`).
- **64-Bit Extended WebSockets**: Upgraded the `websocket_stream_thread` framer (Opcode 127) to calculate and construct 64-bit Payload Length headers, enabling the engine to effortlessly blast megabytes of JSON data over the TCP socket.
- **Live Stream Viewer**: The Web Dashboard renders the massive JSON payload (`{"cpu": %, "ram": %, "screen": "data:image/bmp;base64,..."}`) directly onto the HTML canvas, transforming the browser into a fully functional 5Hz Remote Desktop!

## 🌟 Version 4.4: The Live System Monitor Expansion
**Status:** Released
- **Windows Kernel Hardware APIs (`<windows.h>`)**: Engineered `get_cpu_usage()` and `get_ram_usage()` utilizing raw `GetSystemTimes()` and `GlobalMemoryStatusEx()` to extract the physical hardware metrics from the operating system natively in C.
- **WebSocket Streaming Redesign**: Upgraded the `websocket_stream_thread` to blast the hardware JSON payloads (`{"cpu": %, "ram": %}`) at 10 Frames Per Second over the `ws://` TCP socket.
- **Dual-Line Task Manager Graph**: The web dashboard now features a massive red/blue live chart rendering the live streams perfectly natively in the browser without reloading!

## 🌟 Version 4.3: The Real-Time WebSocket Expansion
**Status:** Released
- **Native C WebSockets (`ws://`)**: Upgraded the C HTTP Server to natively accept `Upgrade: websocket` protocols.
- **Wincrypt.h Handshake Protocol**: The C engine natively intercepts the `Sec-WebSocket-Key`, appends the GUID, hashes it with **SHA-1**, and encodes it via **Base64** using native Windows Cryptography APIs.
- **High-Speed Data Streaming (20Hz)**: Spawns an independent C thread that generates native WebSocket Data Frames (Opcode `0x81`) and blasts live dynamically shifting mathematical data to the Web Dashboard 20 times per second!

## 🌟 Version 4.2: The Ultimate Web Expansion
**Status:** Released
- **C-Powered Engineering Calculus:** Exposed a new `/api/calculus` endpoint. The web dashboard dynamically graphs the Numerical Integral (Area Under Curve) and exact Derivatives using the Trapezoidal Rule and Central Difference Theorem calculated purely in C.
- **Cybersecurity Cryptography (SHA-256):** Added a `/api/crypto` API. The frontend can now request data integrity hashes, which are physically calculated by the C Engine's native SHA-256 logic and returned to the browser.
- **Big Data Compression (RLE):** Added a `/api/compress` API. Users can input massive data arrays into the web dashboard, and the C server will compress them down to the byte level natively via Run-Length Encoding.

## 🌟 Version 4.1: The Unified Web Backend
**Status:** Released
- **Advanced C API Microservice (`server.c`):** Upgraded the C `.exe` Server into a comprehensive API capable of intercepting historical data natively on Port 8080 and crunching Predictive AI and Quantitative Financial Algorithms.
- **Frontend Thin-Client Integration (`web-app/script.js`):** Engineered the Web Application to act as a Thin Client. It now executes asynchronous `fetch()` API calls directly back to the `.exe`, rendering the C-computed variables dynamically onto `Chart.js` graphs.
- **Unified File Serving:** The C server now directly hosts the massive multi-tab `web-app/` directory (serving `index.html`, `style.css`, and `script.js` natively to the browser).

## 🌐 Version 4.0: The Global Web App Release
**Status:** Released
- **Standalone Web Architecture (`web-app/`):** Engineered a 100% independent Client-Side Web Application. Users can now run Calc-Avg natively in the browser without requiring the C `.exe` server. Perfect for GitHub Pages deployment.
- **JavaScript Math Engine:** Ported the core C intelligence into high-performance JS, natively calculating Advanced Statistics, Financial SMA, and Predictive AI (Linear Regression).
- **Multi-Tab Dashboard:** Built an ultra-premium Glassmorphism interface featuring animated orbs, glowing inputs, and advanced `Chart.js` integrations across all mathematical modes.

## 🏆 Version 3.2: The Premium Metadata Update
**Status:** Released
- **Windows Portable Executable (PE) Metadata Injection:** Compiled a native C-Resource script (`calc_avg.rc`) containing the official Windows `VERSIONINFO` block.
- **Embedded Properties:** Hovering over `calc_avg.exe` in Windows natively displays the Company Name (**Sanskar**), File Description, Copyrights, and Version Number (3.2.0.0).
- **`windres` Build Integration:** Overhauled the GNU/MinGW `Makefile` to seamlessly compile and link the `.rc` resource object into the final binary.

## 🌐 Version 3.1: The Web REST API & Dashboard Update
**Status:** Released
- **JSON REST API Microservice (`server.c`):** Transformed the C backend into a modern web API. The `/api/calculate` endpoint dynamically computes advanced statistical calculations (Mean, Median, Mode, Variance, Standard Deviation) and generates structured JSON string responses.
- **Modern Web Dashboard Frontend (`web/`):** Engineered a stunning frontend interface utilizing modern Glassmorphism aesthetics, animated background orbs, and premium typography. The UI operates asynchronously using modern JavaScript `fetch()` APIs to communicate with the C backend without page reloads.
- **Chart.js Integration (`app.js`):** The web dashboard automatically draws beautiful, animated Bar Charts mapping out the numerical data arrays natively inside the browser!

## 🌐 Version 3.0: The Web Server & Sonification Update
**Status:** Released
- **Native HTTP Web Server (`server.c`):** Embedded a native TCP HTTP socket server using `<winsock2.h>`. The application can now host a beautiful HTML/CSS frontend on `localhost:8080`, intercept browser HTTP `GET` requests, dynamically calculate the average, and serve the results natively.
- **Hardware Data Sonification (`audio.c`):** Embedded the low-level Windows kernel API `Beep()`. The engine now maps massive numerical data arrays into physical audio frequencies (Hertz), physically playing the data graph as a synthesized musical tone sequence through the computer hardware.

## 🔐 Version 2.10: The Cybersecurity & Compression Update
**Status:** Released
- **SHA-256 Military Cryptography (`crypto.c`):** Engineered a standalone pure C implementation of the SHA-256 algorithm. Averages are mathematically encrypted using bitwise operations to generate irreversible 64-character hash signatures.
- **Run-Length Big Data Compression:** Deployed an active interception algorithm that compresses repetitive data arrays down to the raw byte level (RLE), drastically optimizing RAM usage and calculating perfect averages from the compressed byte stream.

## 🤖 Version 2.9: The Artificial Intelligence Update
**Status:** Released
- **Machine Learning (Predictive Linear Regression):** The engine mathematically calculates the Slope and Y-Intercept of a dataset to construct a trained predictive Line of Best Fit (`y = mx + b`), accurately projecting numerical forecasts for future time periods.

## 🪟 Version 2.8: The Graphical User Interface (GUI) Update
**Status:** Released
- **Native Windows GUI (`gui.c`):** Bypassed the terminal to summon a true native Windows Application using `<windows.h>` (`CreateWindowEx`). Complete with interactive text boxes, physical buttons, popup Message Boxes, and real-time visual output.

## 📐 Version 2.7: The Engineering Calculus Update
**Status:** Released
- **Numerical Integrals:** Implemented the Trapezoidal Rule to instantly calculate the accumulating total area under the curve of any dataset.
- **True Derivatives:** Implemented the Central Difference Theorem to calculate the exact instantaneous rate of change across numerical data points.

## 📈 Version 2.6: The Financial Markets Update
**Status:** Released
- **Simple Moving Average (SMA):** Integrated Wall-Street quantitative algorithms to calculate momentum across specified sliding window periods.
- **Exponential Moving Average (EMA):** Applied dynamically weighted multipliers to calculate the exponentially smoothed market momentum across financial price arrays.

## 🧊 Version 2.5: The Matrix Mathematics Update
**Status:** Released
- **2D Matrices:** Integrated multi-dimensional arrays, allowing the user to map complete X/Y grids. The engine simultaneously calculates and isolates the independent average of every Row, every Column, and the overall Matrix.

## 🗃️ Version 2.4: The SQLite Database Integration
**Status:** Released
- **Native SQL Queries:** Integrated the official open-source `sqlite3.c` engine. The C binary parses actual `.sqlite` and `.db` database files, extracts specific numerical columns using SQL `SELECT` parameters, and calculates the exact average without CSV extraction.

## ⚡ Version 2.3: The Multi-Threaded Parallel Processing Update
**Status:** Released
- **Hardware Core Utilization:** Implemented Windows `<windows.h>` Threading APIs. The C engine now spawns independent, parallel CPU threads for every single dataset when batch processing massive files simultaneously.

## 🌐 Version 2.2: The Live Internet API Networking Update
**Status:** Released
- **Winsock API Streaming:** Enabled the C engine to open outbound TCP sockets on Port 80 to ping raw internet endpoints. The application natively strips HTTP headers, extracts raw JSON payloads, and calculates the averages of live cloud data streams.

## 📄 Version 2.1: The HTML Report Generator
**Status:** Released
- **Automated Web Documentation:** The application dynamically generates and writes fully responsive `report.html` pages containing the calculated Mean, Median, Mode, Variance, and Standard Deviation inside beautifully formatted CSS tables.

## 🚀 Version 1.0 - 2.0: The Foundation & Advanced Statistics
**Status:** Released
- **Core Statistical Logic:** Implementation of Mean, Median, Mode, Geometric Mean, Harmonic Mean, Variance, and Standard Deviation.
- **Generators:** Integration of natural number generation, prime number validation algorithms, and Fibonacci sequences.
- **File Parsing:** Custom-built tokenization engines to parse raw Data Text Files, Comma-Separated Values (`.csv`), and JSON state machines.
- **Visuals:** Implemented full ANSI-color terminal graphics and the iconic green ASCII Bar Chart `(████████)` magnitude visualizer.

---
*Calc-Avg is a 100% open-source C framework developed by Sanskar.*

## Versions
- **v5.0.0.0** - The Final Polish (Masterpiece Edition - Complete Web Dashboard UI/UX)
- **v4.9.0.0** - The POSIX Cross-Platform Expansion (Linux/macOS Support via Pthreads/Sockets)
- **v4.8.0.0** - The Core Engine Optimization (Dynamic Neural Net Heap Allocation & JSON Parser Overhaul)

- [v4.8 - The Core Engine Optimization](v4.8.md)
