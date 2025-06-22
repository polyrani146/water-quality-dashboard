import React from 'react';
import { Link } from 'react-router-dom';

const Welcome: React.FC = () => {
    return (
        <div className="welcome-container">
            <h1>Welcome to the Water Quality Dashboard</h1>
            <p>Your one-stop solution for monitoring water quality metrics.</p>
            <div className="button-container">
                <Link to="/login">
                    <button>Login</button>
                </Link>
                <Link to="/signup">
                    <button>Sign Up</button>
                </Link>
            </div>
        </div>
    );
};

export default Welcome;