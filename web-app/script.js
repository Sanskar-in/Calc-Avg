/* =========================================================================
   Version 4.0 JavaScript Math Engine & UI Controller
   Ports the C-Backend logic directly into the Client-Side Browser!
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

    // --- Helper: Parse CSV Text to Float Array ---
    const parseData = (str) => {
        const tokens = str.replace(/[^0-9.,-]/g, ' ').split(/[\s,]+/);
        const numbers = [];
        for (let t of tokens) {
            if (t.trim() !== '') {
                const num = parseFloat(t);
                if (!isNaN(num)) numbers.push(num);
            }
        }
        return numbers;
    };

    // =======================================================
    // TAB 1: Advanced Statistical Analysis
    // =======================================================
    document.getElementById('stats-form').addEventListener('submit', (e) => {
        e.preventDefault();
        const dataStr = document.getElementById('stats-data').value;
        const numbers = parseData(dataStr);
        const container = document.getElementById('stats-results');
        const grid = document.getElementById('stats-grid-container');

        if (numbers.length === 0) {
            alert("Please enter valid numerical data.");
            return;
        }

        // Mathematical Computations (Ported from C)
        const count = numbers.length;
        const sum = numbers.reduce((a, b) => a + b, 0);
        const mean = sum / count;

        // Median
        const sorted = [...numbers].sort((a, b) => a - b);
        const median = count % 2 === 0 
            ? (sorted[count/2 - 1] + sorted[count/2]) / 2 
            : sorted[Math.floor(count/2)];

        // Variance & Std Dev (Population)
        const varSum = numbers.reduce((a, b) => a + Math.pow(b - mean, 2), 0);
        const variance = varSum / count;
        const stdDev = Math.sqrt(variance);

        // Rendering UI
        container.classList.remove('hidden');
        grid.innerHTML = `
            <div class="stat-card">
                <div class="stat-title">N (Count)</div>
                <div class="stat-value" style="color: #94a3b8">${count}</div>
            </div>
            <div class="stat-card">
                <div class="stat-title">Arithmetic Mean</div>
                <div class="stat-value" style="color: var(--primary-color)">${mean.toFixed(4)}</div>
            </div>
            <div class="stat-card">
                <div class="stat-title">Median</div>
                <div class="stat-value" style="color: #38bdf8">${median.toFixed(4)}</div>
            </div>
            <div class="stat-card">
                <div class="stat-title">Variance</div>
                <div class="stat-value" style="color: #f472b6">${variance.toFixed(4)}</div>
            </div>
            <div class="stat-card">
                <div class="stat-title">Standard Deviation</div>
                <div class="stat-value" style="color: #fbbf24">${stdDev.toFixed(4)}</div>
            </div>
        `;

        // Render Chart.js
        if (statsChartInstance) statsChartInstance.destroy();
        const ctx = document.getElementById('statsChart').getContext('2d');
        Chart.defaults.color = '#a1a1aa';
        Chart.defaults.font.family = "'Inter', sans-serif";
        
        statsChartInstance = new Chart(ctx, {
            type: 'bar',
            data: {
                labels: numbers.map((_, i) => `P${i+1}`),
                datasets: [{
                    label: 'Dataset Values',
                    data: numbers,
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
    });

    // =======================================================
    // TAB 2: Predictive AI (Linear Regression)
    // =======================================================
    document.getElementById('ml-form').addEventListener('submit', (e) => {
        e.preventDefault();
        const dataStr = document.getElementById('ml-data').value;
        const futureCount = parseInt(document.getElementById('ml-future').value);
        const y_vals = parseData(dataStr);
        const container = document.getElementById('ml-results');
        const eqBox = document.getElementById('ml-equation');

        if (y_vals.length < 2) {
            alert("AI needs at least 2 historical data points to train a model.");
            return;
        }

        const n = y_vals.length;
        const x_vals = Array.from({length: n}, (_, i) => i + 1);

        let sum_x = 0, sum_y = 0, sum_xy = 0, sum_x_sq = 0;
        for (let i = 0; i < n; i++) {
            sum_x += x_vals[i];
            sum_y += y_vals[i];
            sum_xy += x_vals[i] * y_vals[i];
            sum_x_sq += x_vals[i] * x_vals[i];
        }

        const denominator = (n * sum_x_sq) - (sum_x * sum_x);
        let m = 0, b = 0;
        
        if (denominator !== 0) {
            m = ((n * sum_xy) - (sum_x * sum_y)) / denominator;
            b = (sum_y - (m * sum_x)) / n;
        }

        // Generate predictions
        const future_x = [];
        const future_y = [];
        for (let i = 1; i <= futureCount; i++) {
            const x = n + i;
            future_x.push(x);
            future_y.push(m * x + b);
        }

        container.classList.remove('hidden');
        eqBox.innerHTML = `Trained AI Model: <strong style="color:#fff">y = ${m.toFixed(4)}x + ${b.toFixed(4)}</strong>`;

        // Render AI Chart
        if (mlChartInstance) mlChartInstance.destroy();
        const ctx = document.getElementById('mlChart').getContext('2d');
        
        // Generate trendline for historical data
        const historical_trend = x_vals.map(x => m * x + b);

        mlChartInstance = new Chart(ctx, {
            type: 'line',
            data: {
                labels: [...x_vals, ...future_x].map(x => `T${x}`),
                datasets: [
                    {
                        label: 'Historical Data',
                        data: [...y_vals, ...Array(futureCount).fill(null)],
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
    });

    // =======================================================
    // TAB 3: Financial SMA (Simple Moving Average)
    // =======================================================
    document.getElementById('sma-form').addEventListener('submit', (e) => {
        e.preventDefault();
        const dataStr = document.getElementById('sma-data').value;
        const window = parseInt(document.getElementById('sma-window').value);
        const prices = parseData(dataStr);
        const container = document.getElementById('sma-results');

        if (prices.length < window) {
            alert(`Insufficient data. You need at least ${window} prices for a Window of ${window}.`);
            return;
        }

        const sma = [];
        // Fill initial nulls
        for(let i = 0; i < window - 1; i++) sma.push(null);

        for (let i = 0; i <= prices.length - window; i++) {
            let sum = 0;
            for (let j = 0; j < window; j++) {
                sum += prices[i + j];
            }
            sma.push(sum / window);
        }

        container.classList.remove('hidden');

        // Render SMA Chart
        if (smaChartInstance) smaChartInstance.destroy();
        const ctx = document.getElementById('smaChart').getContext('2d');
        
        smaChartInstance = new Chart(ctx, {
            type: 'line',
            data: {
                labels: prices.map((_, i) => `Day ${i+1}`),
                datasets: [
                    {
                        label: 'Asset Price',
                        data: prices,
                        borderColor: 'rgba(255, 255, 255, 0.3)',
                        tension: 0.1
                    },
                    {
                        label: `SMA (${window}-Day)`,
                        data: sma,
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
    });

});
