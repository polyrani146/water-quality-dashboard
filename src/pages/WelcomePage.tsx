import React from 'react';
import { Link } from 'react-router-dom';
import Welcome from '../components/Welcome';

const WelcomePage: React.FC = () => {
    return (
        <div>
            <Welcome />
            <div>
                <Link to="/login">Login</Link>
                <Link to="/signup">Sign Up</Link>
            </div>
        </div>
    );
};

export default WelcomePage;