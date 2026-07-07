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
            document.getElementById(btn.dataset.tab).classList.add('active');
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
        
        wsStatus.innerText = "Connecting...";
        wsStatus.style.color = "#fbbf24"; // yellow
        
        ws = new WebSocket(`ws://${window.location.host}`);
        
        ws.onopen = () => {
            wsStatus.innerText = "Connected (10Hz Live)";
            wsStatus.style.color = "#10b981"; // green
            startBtn.style.display = "none";
            stopBtn.style.display = "inline-block";
        };
        
        let counter = 0;
        ws.onmessage = (event) => {
            try {
                const data = JSON.parse(event.data);
                wsCpu.innerText = data.cpu.toFixed(2) + '%';
                wsRam.innerText = data.ram.toFixed(2) + '%';
                
                counter++;
                // Add to chart
                streamChart.data.labels.push(counter);
                streamChart.data.datasets[0].data.push(data.cpu);
                streamChart.data.datasets[1].data.push(data.ram);
                
                // Keep only the last 60 points so it scrolls
                if (streamChart.data.labels.length > 60) {
                    streamChart.data.labels.shift();
                    streamChart.data.datasets[0].data.shift();
                    streamChart.data.datasets[1].data.shift();
                }
                
                streamChart.update();
            } catch (e) {
                console.error("Invalid WebSocket Frame:", event.data);
            }
        };
        
        ws.onclose = () => {
            wsStatus.innerText = "Disconnected.";
            wsStatus.style.color = "#f43f5e"; // red
            startBtn.style.display = "inline-block";
            stopBtn.style.display = "none";
        };
        
        ws.onerror = (err) => {
            console.error("WebSocket Error:", err);
            wsStatus.innerText = "Error!";
            wsStatus.style.color = "#f43f5e"; // red
        };
    });
    
    stopBtn.addEventListener('click', () => {
        if (ws) ws.close();
    });

});
