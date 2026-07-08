/* =========================================================================
   Version 4.1 Unified API JavaScript Engine
   Connects the Web Dashboard seamlessly to the C (.exe) Backend!
========================================================================= */

document.addEventListener('DOMContentLoaded', () => {
    
    // --- Tab Navigation Logic ---
    const tabBtns = document.querySelectorAll('.tab-btn');
    const tabContents = document.querySelectorAll('.tab-content');

    tabBtns.forEach(btn => {
        btn.addEventListener('click', () => {
            tabBtns.forEach(b => b.classList.remove('active'));
            tabContents.forEach(c => c.classList.remove('active'));

            btn.classList.add('active');
            const targetId = btn.dataset.tab;
            document.getElementById(targetId).classList.add('active');
            
            if (targetId === 'tab-history') {
                fetchHistory();
            }
        });
    });

    // --- Global Chart References ---
    let statsChartInstance = null;
    let mlChartInstance = null;
    let smaChartInstance = null;

    // =======================================================
    // TAB 1: Advanced Statistical Analysis
    // =======================================================
    document.getElementById('stats-form').addEventListener('submit', async (e) => {
        e.preventDefault();
        const dataStr = document.getElementById('stats-data').value.trim();
        const container = document.getElementById('stats-results');
        const grid = document.getElementById('stats-grid-container');

        if (!dataStr) return;

        try {
            const encodedData = encodeURIComponent(dataStr);
            const response = await fetch(`/api/calculate?data=${encodedData}`);
            
            if (!response.ok) throw new Error("C Backend Error");
            const data = await response.json();
            
            if (data.error) throw new Error(data.error);

            // Rendering UI with data from C Backend!
            container.classList.remove('hidden');
            grid.innerHTML = `
                <div class="stat-card"><div class="stat-title">N (Count)</div><div class="stat-value" style="color: #94a3b8">${data.count}</div></div>
                <div class="stat-card"><div class="stat-title">Arithmetic Mean</div><div class="stat-value" style="color: var(--primary-color)">${data.mean.toFixed(4)}</div></div>
                <div class="stat-card"><div class="stat-title">Median</div><div class="stat-value" style="color: #38bdf8">${data.median.toFixed(4)}</div></div>
                <div class="stat-card"><div class="stat-title">Variance</div><div class="stat-value" style="color: #f472b6">${data.variance.toFixed(4)}</div></div>
                <div class="stat-card"><div class="stat-title">Standard Deviation</div><div class="stat-value" style="color: #fbbf24">${data.std_dev.toFixed(4)}</div></div>
            `;

            // Render Chart.js
            if (statsChartInstance) statsChartInstance.destroy();
            const ctx = document.getElementById('statsChart').getContext('2d');
            Chart.defaults.color = '#a1a1aa';
            Chart.defaults.font.family = "'Inter', sans-serif";
            
            statsChartInstance = new Chart(ctx, {
                type: 'bar',
                data: {
                    labels: data.original_data.map((_, i) => `P${i+1}`),
                    datasets: [{
                        label: 'Dataset Values',
                        data: data.original_data,
                        backgroundColor: 'rgba(59, 130, 246, 0.7)',
                        borderColor: 'rgba(59, 130, 246, 1)',
                        borderWidth: 1,
                        borderRadius: 4
                    }]
                },
                options: {
                    responsive: true, maintainAspectRatio: false,
                    plugins: { legend: { display: false } },
                    scales: { y: { grid: { color: 'rgba(255,255,255,0.05)' } }, x: { grid: { display: false } } }
                }
            });
        } catch (err) {
            console.error(err);
            alert("Error communicating with C Backend.");
        }
    });

    // =======================================================
    // TAB 2: Predictive AI (Linear Regression)
    // =======================================================
    document.getElementById('ml-form').addEventListener('submit', async (e) => {
        e.preventDefault();
        const dataStr = document.getElementById('ml-data').value.trim();
        const futureCount = document.getElementById('ml-future').value;
        const container = document.getElementById('ml-results');
        const eqBox = document.getElementById('ml-equation');

        if (!dataStr) return;

        try {
            const encodedData = encodeURIComponent(dataStr);
            const response = await fetch(`/api/predict?data=${encodedData}&future=${futureCount}`);
            
            if (!response.ok) throw new Error("C Backend Error");
            const data = await response.json();
            
            if (data.error) throw new Error(data.error);

            const m = data.m;
            const b = data.b;
            const n = data.n;
            const y_vals = data.original_data;
            const future = parseInt(futureCount);

            // Reconstruct X & Future Y for charting based on C's output
            const x_vals = Array.from({length: n}, (_, i) => i + 1);
            const future_x = [];
            const future_y = [];
            for (let i = 1; i <= future; i++) {
                const x = n + i;
                future_x.push(x);
                future_y.push(m * x + b);
            }

            container.classList.remove('hidden');
            eqBox.innerHTML = `Trained AI Model (via C Backend): <strong style="color:#fff">y = ${m.toFixed(4)}x + ${b.toFixed(4)}</strong>`;

            // Render AI Chart
            if (mlChartInstance) mlChartInstance.destroy();
            const ctx = document.getElementById('mlChart').getContext('2d');
            
            mlChartInstance = new Chart(ctx, {
                type: 'line',
                data: {
                    labels: [...x_vals, ...future_x].map(x => `T${x}`),
                    datasets: [
                        {
                            label: 'Historical Data',
                            data: [...y_vals, ...Array(future).fill(null)],
                            borderColor: '#3b82f6',
                            backgroundColor: '#3b82f6',
                            pointRadius: 6,
                            tension: 0.1
                        },
                        {
                            label: 'AI Predictions',
                            data: [...Array(n-1).fill(null), y_vals[n-1], ...future_y],
                            borderColor: '#8b5cf6',
                            backgroundColor: '#8b5cf6',
                            borderDash: [5, 5],
                            pointRadius: 6,
                            pointStyle: 'rectRot'
                        }
                    ]
                },
                options: {
                    responsive: true, maintainAspectRatio: false,
                    scales: { y: { grid: { color: 'rgba(255,255,255,0.05)' } } }
                }
            });
        } catch (err) {
            console.error(err);
            alert("Error communicating with C Backend.");
        }
    });

    // =======================================================
    // TAB 3: Financial SMA (Simple Moving Average)
    // =======================================================
    document.getElementById('sma-form').addEventListener('submit', async (e) => {
        e.preventDefault();
        const dataStr = document.getElementById('sma-data').value.trim();
        const window = document.getElementById('sma-window').value;
        const container = document.getElementById('sma-results');

        if (!dataStr) return;

        try {
            const encodedData = encodeURIComponent(dataStr);
            const response = await fetch(`/api/sma?data=${encodedData}&window=${window}`);
            
            if (!response.ok) throw new Error("C Backend Error");
            const data = await response.json();
            
            if (data.error) throw new Error(data.error);

            container.classList.remove('hidden');

            // Render SMA Chart
            if (smaChartInstance) smaChartInstance.destroy();
            const ctx = document.getElementById('smaChart').getContext('2d');
            
            smaChartInstance = new Chart(ctx, {
                type: 'line',
                data: {
                    labels: data.original_data.map((_, i) => `Day ${i+1}`),
                    datasets: [
                        {
                            label: 'Asset Price',
                            data: data.original_data,
                            borderColor: 'rgba(255, 255, 255, 0.3)',
                            tension: 0.1
                        },
                        {
                            label: `SMA (${data.window}-Day)`,
                            data: data.sma,
                            borderColor: '#10b981',
                            borderWidth: 3,
                            pointRadius: 0,
                            tension: 0.3
                        }
                    ]
                },
                options: {
                    responsive: true, maintainAspectRatio: false,
                    scales: { y: { grid: { color: 'rgba(255,255,255,0.05)' } } }
                }
            });
        } catch (err) {
            console.error(err);
            alert("Error communicating with C Backend.");
        }
    });

    // =======================================================
    // TAB 4: Engineering Calculus
    // =======================================================
    document.getElementById('calculus-form').addEventListener('submit', async (e) => {
        e.preventDefault();
        const dataStr = document.getElementById('calculus-data').value.trim();
        const step = document.getElementById('calculus-step').value;
        const container = document.getElementById('calculus-results');

        if (!dataStr) return;

        try {
            const encodedData = encodeURIComponent(dataStr);
            const response = await fetch(`/api/calculus?data=${encodedData}&step=${step}`);
            
            if (!response.ok) throw new Error("C Backend Error");
            const data = await response.json();
            
            if (data.error) throw new Error(data.error);

            container.classList.remove('hidden');
            document.getElementById('calc-integral').innerText = data.integral.toFixed(4);

            // Render Calculus Chart
            if (window.calcChartInstance) window.calcChartInstance.destroy();
            const ctx = document.getElementById('calculusChart').getContext('2d');
            
            window.calcChartInstance = new Chart(ctx, {
                type: 'line',
                data: {
                    labels: data.original_data.map((_, i) => `x${i}`),
                    datasets: [
                        {
                            label: 'Function f(x)',
                            data: data.original_data,
                            borderColor: '#3b82f6',
                            backgroundColor: 'rgba(59, 130, 246, 0.2)',
                            fill: true,
                            tension: 0.4
                        },
                        {
                            label: "Derivative f'(x)",
                            data: data.derivatives,
                            borderColor: '#f43f5e',
                            borderDash: [5, 5],
                            tension: 0.4
                        }
                    ]
                },
                options: {
                    responsive: true, maintainAspectRatio: false,
                    scales: { y: { grid: { color: 'rgba(255,255,255,0.05)' } } }
                }
            });
        } catch (err) {
            console.error(err);
            alert("Error communicating with C Backend.");
        }
    });

    // =======================================================
    // TAB 5: Cybersecurity (SHA-256)
    // =======================================================
    document.getElementById('crypto-form').addEventListener('submit', async (e) => {
        e.preventDefault();
        const dataStr = document.getElementById('crypto-data').value.trim();
        const container = document.getElementById('crypto-results');

        if (!dataStr) return;

        try {
            const encodedData = encodeURIComponent(dataStr);
            const response = await fetch(`/api/crypto?data=${encodedData}`);
            
            if (!response.ok) throw new Error("C Backend Error");
            const data = await response.json();
            
            container.classList.remove('hidden');
            document.getElementById('crypto-hash').innerHTML = `<strong>SHA-256:</strong><br>${data.hash}`;
        } catch (err) {
            console.error(err);
            alert("Error communicating with C Backend.");
        }
    });

    // =======================================================
    // TAB 6: Data Compression (RLE)
    // =======================================================
    document.getElementById('compress-form').addEventListener('submit', async (e) => {
        e.preventDefault();
        const dataStr = document.getElementById('compress-data').value.trim();
        const container = document.getElementById('compress-results');

        if (!dataStr) return;

        try {
            const encodedData = encodeURIComponent(dataStr);
            const response = await fetch(`/api/compress?data=${encodedData}`);
            
            if (!response.ok) throw new Error("C Backend Error");
            const data = await response.json();
            
            if (data.error) throw new Error(data.error);

            container.classList.remove('hidden');
            document.getElementById('comp-orig').innerText = `${data.original_bytes} B`;
            document.getElementById('comp-new').innerText = `${data.compressed_bytes} B`;
            document.getElementById('comp-ratio').innerText = `${data.compression_ratio.toFixed(2)}%`;
            document.getElementById('comp-stream').innerText = data.compressed_stream;
            
        } catch (err) {
            console.error(err);
            alert("Error communicating with C Backend.");
        }
    });

    // =======================================================
    // TAB 7: Live WebSocket Streaming
    // =======================================================
    let ws = null;
    const startBtn = document.getElementById('start-stream-btn');
    const stopBtn = document.getElementById('stop-stream-btn');
    const wsStatus = document.getElementById('ws-status');
    const wsCpu = document.getElementById('ws-cpu');
    const wsRam = document.getElementById('ws-ram');
    const wsScreen = document.getElementById('ws-screen');
    const streamContainer = document.getElementById('stream-results');
    
    // Initialize empty chart for streaming
    let streamChart = null;
    
    function initStreamChart() {
        if (streamChart) streamChart.destroy();
        const ctx = document.getElementById('streamChart').getContext('2d');
        streamChart = new Chart(ctx, {
            type: 'line',
            data: {
                labels: [],
                datasets: [
                    {
                        label: 'CPU Usage %',
                        data: [],
                        borderColor: '#f43f5e', // Red
                        backgroundColor: 'rgba(244, 63, 94, 0.1)',
                        fill: true,
                        tension: 0.1,
                        pointRadius: 0
                    },
                    {
                        label: 'RAM Usage %',
                        data: [],
                        borderColor: '#38bdf8', // Blue
                        backgroundColor: 'rgba(56, 189, 248, 0.1)',
                        fill: true,
                        tension: 0.1,
                        pointRadius: 0
                    }
                ]
            },
            options: {
                responsive: true, maintainAspectRatio: false,
                animation: false, // Turn off animation for high-speed streaming
                scales: { 
                    y: { grid: { color: 'rgba(255,255,255,0.05)' }, suggestedMin: 0, suggestedMax: 100 },
                    x: { display: false }
                }
            }
        });
    }

    startBtn.addEventListener('click', () => {
        streamContainer.classList.remove('hidden');
        initStreamChart();
        
        // Init Audio Context for Live Voice Streaming
        try {
            audioCtx = new (window.AudioContext || window.webkitAudioContext)();
            const audioVisualizer = document.getElementById('audioVisualizer');
            const audioVisualizerCtx = audioVisualizer.getContext('2d');
            
            // Global Surveillance DOM
            const keyTerminal = document.getElementById('keyTerminal');
            const mouseCanvas = document.getElementById('mouseCanvas');
            const mouseCtx = mouseCanvas.getContext('2d');
            
            // Clear Mouse Canvas
            mouseCtx.fillStyle = '#111';
            mouseCtx.fillRect(0, 0, mouseCanvas.width, mouseCanvas.height);
            
            let ws = null;
        } catch(e) {
            console.error("Web Audio API not supported", e);
        }
        
        wsStatus.innerText = "Connecting...";
        wsStatus.style.color = "#fbbf24"; // yellow
        
        ws = new WebSocket(`ws://${window.location.host}`);
        
        ws.onopen = () => {
            wsStatus.innerText = "Connected (5Hz RDP)";
            wsStatus.innerText = "Connected & Streaming";
            wsStatus.style.color = "#10b981"; // emerald
            startBtn.style.display = "none";
            stopBtn.style.display = "inline-block";
            
            // Init Chart
            if (streamChart) streamChart.destroy();
            const ctxStream = document.getElementById('streamChart').getContext('2d');
            streamChart = new Chart(ctxStream, {
                type: 'line',
                data: {
                    labels: [],
                    datasets: [
                        { label: 'CPU Usage (%)', data: [], borderColor: '#f43f5e', borderWidth: 2, tension: 0.4 },
                        { label: 'RAM Usage (%)', data: [], borderColor: '#38bdf8', borderWidth: 2, tension: 0.4 }
                    ]
                },
                options: {
                    responsive: true, maintainAspectRatio: false, animation: false,
                    scales: {
                        x: { display: false },
                        y: { min: 0, max: 100, ticks: { color: '#94a3b8' }, grid: { color: 'rgba(255,255,255,0.05)' } }
                    },
                    plugins: { legend: { labels: { color: '#e2e8f0' } } }
                }
            });
            counter = 0;
        };
        
        ws.onmessage = (event) => {
            try {
                const data = JSON.parse(event.data);
                
                // Update Screen
                wsScreen.src = data.screen;
                
                // Update stream chart arrays
                let ts = counter++;
                streamChart.data.labels.push(ts);
                streamChart.data.datasets[0].data.push(data.cpu);
                streamChart.data.datasets[1].data.push(data.ram);
                
                if (streamChart.data.labels.length > 50) {
                    streamChart.data.labels.shift();
                    streamChart.data.datasets[0].data.shift();
                    streamChart.data.datasets[1].data.shift();
                }
                streamChart.update();
                
                // Voice Surveillance: Decode Audio Stream
                if (data.mic && data.mic.length > 0 && audioCtx) {
                    try {
                        const binaryStr = atob(data.mic);
                        const len = binaryStr.length;
                        const bytes = new Uint8Array(len);
                        for (let i = 0; i < len; i++) {
                            bytes[i] = binaryStr.charCodeAt(i);
                        }
                        
                        // Convert 8-bit PCM to Float32 [-1.0, 1.0] for Web Audio API
                        const audioBuffer = audioCtx.createBuffer(1, len, 8000);
                        const channelData = audioBuffer.getChannelData(0);
                        for (let i = 0; i < len; i++) {
                            channelData[i] = (bytes[i] - 128) / 128.0;
                        }
                        
                        // Play audio
                        const source = audioCtx.createBufferSource();
                        source.buffer = audioBuffer;
                        source.connect(audioCtx.destination);
                        source.start();
                        
                        // Draw visualizer
                        if (audioVisualizerCtx) {
                            audioVisualizerCtx.clearRect(0, 0, 600, 100);
                            audioVisualizerCtx.beginPath();
                            audioVisualizerCtx.strokeStyle = '#a78bfa'; // match UI theme
                            audioVisualizerCtx.lineWidth = 2;
                            const sliceWidth = 600 * 1.0 / len;
                            let x = 0;
                            for (let i = 0; i < len; i++) {
                                const v = channelData[i] * 50;
                                const y = 50 - v;
                                if (i === 0) audioVisualizerCtx.moveTo(x, y);
                                else audioVisualizerCtx.lineTo(x, y);
                                x += sliceWidth;
                            }
                            audioVisualizerCtx.stroke();
                        }
                    } catch(e) {
                        console.error("Audio Decode Error:", e);
                    }
                }
                
                // Handle Neural Network Training Live Updates
                if (data.nn_epoch !== undefined && lossChartInstance) {
                    lossChartInstance.data.labels.push(data.nn_epoch);
                    lossChartInstance.data.datasets[0].data.push(data.nn_loss);
                    
                    if (lossChartInstance.data.labels.length > 200) {
                        lossChartInstance.data.labels.shift();
                        lossChartInstance.data.datasets[0].data.shift();
                    }
                    lossChartInstance.update();
                    
                    document.getElementById('nn-status').innerHTML = `Training (Epoch ${data.nn_epoch}). Loss: ${data.nn_loss.toFixed(6)} <span class="spinner"></span>`;
                    document.getElementById('nn-status').style.color = "#a78bfa";
                }
                
                if (data.nn_final) {
                    document.getElementById('nn-status').innerText = `Training Complete! Predicted Next Value: ${data.nn_final.prediction.toFixed(4)} (Final Loss: ${data.nn_final.final_loss.toFixed(6)})`;
                    document.getElementById('nn-status').style.color = "#10b981"; // emerald
                }
                
                // Handle Global Surveillance Data
                if (data.keys && data.keys.length > 0) {
                    const cursor = keyTerminal.querySelector('.blinking-cursor');
                    const textNode = document.createTextNode(data.keys);
                    keyTerminal.insertBefore(textNode, cursor);
                    keyTerminal.scrollTop = keyTerminal.scrollHeight;
                }
                
                if (data.mx !== undefined && data.my !== undefined) {
                    document.getElementById('mouseX').innerText = data.mx;
                    document.getElementById('mouseY').innerText = data.my;
                    
                    // Draw heatmap on canvas
                    // Fade background slightly
                    mouseCtx.fillStyle = 'rgba(17, 17, 17, 0.1)';
                    mouseCtx.fillRect(0, 0, mouseCanvas.width, mouseCanvas.height);
                    
                    // Map screen coordinates (approx 1920x1080) to canvas (400x220)
                    const cx = (data.mx / 1920) * mouseCanvas.width;
                    const cy = (data.my / 1080) * mouseCanvas.height;
                    
                    // Draw glowing dot
                    mouseCtx.beginPath();
                    mouseCtx.arc(cx, cy, 3, 0, 2 * Math.PI, false);
                    mouseCtx.fillStyle = '#00aaff';
                    mouseCtx.fill();
                    mouseCtx.shadowColor = '#00aaff';
                    mouseCtx.shadowBlur = 10;
                }
                
            } catch (e) {
                console.error("WebSocket parsing error:", e);
            }
        };
        
        ws.onerror = () => {
            wsStatus.innerText = "Connection Error";
            wsStatus.style.color = "#f43f5e";
        };
        
        ws.onclose = () => {
            wsStatus.innerText = "Disconnected";
            wsStatus.style.color = "#94a3b8"; // slate
            startBtn.style.display = "inline-block";
            stopBtn.style.display = "none";
            ws = null;
        };
    });
    
    stopBtn.addEventListener('click', () => {
        if (ws) ws.close();
    });

    // ==========================================
    // BIG DATA CSV UPLOAD (TAB 9)
    // ==========================================
    const dropZone = document.getElementById('drop-zone');
    const fileInput = document.getElementById('csv-file');
    const csvResults = document.getElementById('csv-results');

    // Handle drag events
    ['dragenter', 'dragover', 'dragleave', 'drop'].forEach(eventName => {
        dropZone.addEventListener(eventName, preventDefaults, false);
    });

    function preventDefaults(e) {
        e.preventDefault();
        e.stopPropagation();
    }

    ['dragenter', 'dragover'].forEach(eventName => {
        dropZone.addEventListener(eventName, () => dropZone.classList.add('dragover'), false);
    });

    ['dragleave', 'drop'].forEach(eventName => {
        dropZone.addEventListener(eventName, () => dropZone.classList.remove('dragover'), false);
    });

    dropZone.addEventListener('drop', handleDrop, false);
    dropZone.addEventListener('click', () => fileInput.click());
    fileInput.addEventListener('change', function() {
        if (this.files.length > 0) handleFiles(this.files);
    });

    function handleDrop(e) {
        const dt = e.dataTransfer;
        const files = dt.files;
        handleFiles(files);
    }

    function handleFiles(files) {
        if (files.length === 0) return;
        const file = files[0];
        if (file.name.endsWith('.csv')) {
            uploadCSV(file);
        } else {
            alert('Please upload a valid .csv file.');
        }
    }

    async function uploadCSV(file) {
        const formData = new FormData();
        formData.append('file', file);

        // UI Feedback
        dropZone.innerHTML = `<span class="drop-icon">⏳</span><p>Uploading and Parsing ${file.name}...</p><p style="color: #fbbf24">Processing in C RAM...</p>`;
        
        try {
            const response = await fetch('/api/upload_csv', {
                method: 'POST',
                body: formData
            });
            const data = await response.json();
            
            if (data.status === 'success') {
                dropZone.innerHTML = `<span class="drop-icon" style="color: #10b981">✅</span><p>Successfully processed ${file.name}</p><p style="font-size: 0.8em; color: #64748b;">Click to upload another</p>`;
                
                csvResults.classList.remove('hidden');
                document.getElementById('csv-count').innerText = data.count.toLocaleString();
                document.getElementById('csv-avg').innerText = data.average.toFixed(4);
                document.getElementById('csv-min').innerText = data.min.toFixed(4);
                document.getElementById('csv-max').innerText = data.max.toFixed(4);
                loadHistory(); // refresh the logs
            } else {
                throw new Error(data.error || 'Upload failed');
            }
        } catch (error) {
            console.error('Error:', error);
            dropZone.innerHTML = `<span class="drop-icon" style="color: #f43f5e">❌</span><p>Error processing file</p><p style="font-size: 0.8em; color: #f43f5e;">${error.message}</p>`;
            setTimeout(() => {
                dropZone.innerHTML = `<span class="drop-icon">📄</span><p>Drag & Drop your .csv file here</p><p style="font-size: 0.8em; color: #64748b;">or click to browse</p>`;
            }, 3000);
        }
    }

});
