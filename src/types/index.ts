export interface User {
    id: string;
    name: string;
    email: string;
    password: string;
}

export interface WaterQualityMetrics {
    pH: number;
    turbidity: number; // in NTU
    TDS: number; // in mg/L
    temperature: number; // in °C
}