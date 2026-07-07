document.addEventListener('DOMContentLoaded', () => {
    const form = document.getElementById('calc-form');
    const dataInput = document.getElementById('data-input');
    const opSelect = document.getElementById('operation-select');
    const resultsContainer = document.getElementById('results-container');
    const statsGrid = document.getElementById('stats-grid');
    const submitBtn = form.querySelector('button');
    const btnText = submitBtn.querySelector('span');

    let myChart = null;

    form.addEventListener('submit', async (e) => {
        e.preventDefault();
        
        const rawData = dataInput.value.trim();
        const operation = opSelect.value;
        
        if (!rawData) return;

        // UI Feedback
        btnText.textContent = "Processing via C Engine...";
        submitBtn.style.opacity = '0.8';
        submitBtn.style.pointerEvents = 'none';

        try {
            // Encode the data to handle commas and spaces safely in URL
            const encodedData = encodeURIComponent(rawData);
            
            // Hit the C Backend REST API
            const response = await fetch(`/api/calculate?data=${encodedData}&op=${operation}`);
            
            if (!response.ok) {
                throw new Error(`Server returned status ${response.status}`);
            }

            const result = await response.json();
            
            if (result.error) {
                throw new Error(result.error);
            }

            renderResults(result);
            renderChart(result.original_data);

        } catch (error) {
            console.error("API Error:", error);
            resultsContainer.classList.remove('hidden');
            statsGrid.innerHTML = `<div class="stat-card error-msg">
                <div class="stat-title">Error</div>
                <div class="stat-value" style="font-size:1.2rem; color:var(--danger)">${error.message}</div>
            </div>`;
        } finally {
            // Restore UI
            btnText.textContent = "Initialize C Engine";
            submitBtn.style.opacity = '1';
            submitBtn.style.pointerEvents = 'auto';
        }
    });

    function renderResults(data) {
        resultsContainer.classList.remove('hidden');
        statsGrid.innerHTML = '';

        // Helper to create cards
        const createCard = (title, value, colorClass = '') => {
            const valStr = (typeof value === 'number') ? Number(value.toFixed(4)).toString() : value;
            return `
            <div class="stat-card">
                <div class="stat-title">${title}</div>
                <div class="stat-value" ${colorClass ? `style="color:var(--${colorClass})"` : ''}>${valStr}</div>
            </div>`;
        };

        let html = '';
        html += createCard("Data Points", data.count, "secondary");
        html += createCard("Sum", data.sum);
        
        if (data.mean !== undefined) html += createCard("Arithmetic Mean", data.mean, "success");
        if (data.median !== undefined) html += createCard("Median", data.median);
        if (data.mode !== undefined) html += createCard("Mode", data.mode);
        if (data.variance !== undefined) html += createCard("Variance", data.variance);
        if (data.std_dev !== undefined) html += createCard("Standard Deviation", data.std_dev);

        statsGrid.innerHTML = html;
    }

    function renderChart(dataArray) {
        if (!dataArray || dataArray.length === 0) return;

        const ctx = document.getElementById('dataChart').getContext('2d');
        
        if (myChart) {
            myChart.destroy();
        }

        const labels = dataArray.map((_, i) => `Point ${i + 1}`);
        
        // Setup Chart.js with beautiful modern styling matching our CSS
        Chart.defaults.color = '#94a3b8';
        Chart.defaults.font.family = "'Inter', sans-serif";

        myChart = new Chart(ctx, {
            type: 'bar',
            data: {
                labels: labels,
                datasets: [{
                    label: 'Data Values',
                    data: dataArray,
                    backgroundColor: 'rgba(56, 189, 248, 0.6)',
                    borderColor: 'rgba(56, 189, 248, 1)',
                    borderWidth: 1,
                    borderRadius: 4,
                    hoverBackgroundColor: 'rgba(129, 140, 248, 0.8)'
                }]
            },
            options: {
                responsive: true,
                maintainAspectRatio: false,
                plugins: {
                    legend: { display: false },
                    tooltip: {
                        backgroundColor: 'rgba(15, 23, 42, 0.9)',
                        titleFont: { size: 14, family: "'Outfit', sans-serif" },
                        bodyFont: { size: 14 },
                        padding: 12,
                        cornerRadius: 8,
                        displayColors: false
                    }
                },
                scales: {
                    y: {
                        grid: { color: 'rgba(255, 255, 255, 0.05)' },
                        beginAtZero: true
                    },
                    x: {
                        grid: { display: false }
                    }
                },
                animation: {
                    duration: 1000,
                    easing: 'easeOutQuart'
                }
            }
        });
    }
});
