import React from 'react';

const Dashboard: React.FC = () => {
    // Sample data for water quality metrics
    const waterQualityData = {
        pH: 7.2,
        turbidity: 5.0, // NTU
        TDS: 300, // mg/L
        temperature: 22 // °C
    };

    return (
        <div>
            <h1>Water Quality Dashboard</h1>
            <div>
                <h2>Water Quality Metrics</h2>
                <ul>
                    <li>pH: {waterQualityData.pH}</li>
                    <li>Turbidity: {waterQualityData.turbidity} NTU</li>
                    <li>TDS: {waterQualityData.TDS} mg/L</li>
                    <li>Temperature: {waterQualityData.temperature} °C</li>
                </ul>
            </div>
        </div>
    );
};

export default Dashboard;