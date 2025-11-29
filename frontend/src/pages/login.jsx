// Login Page

import React, { useState } from 'react';
import { useNavigate } from 'react-router-dom';

import '../styles/login.css'
import '../styles/App.css'

function LoginPage({ setLoggedIn }) {
    const [username, setUsername] = useState('');
    const [password, setPassword] = useState('');
    const [message,  setMessage]  = useState('');

    const navigate = useNavigate();

    const handleSubmit = async (e) => {
        e.preventDefault();

        try {
            const res = await fetch("http://localhost:18080/login", {
                method: "POST",
                headers: { "Content-Type": "application/json" },
                body: JSON.stringify({ username, password }),
            });

            const data = await res.json();

            if (data.success) {
                setMessage("Login successful!");
                setLoggedIn(true);     
                setTimeout(() => navigate('/'), 500); // redirect to home after 0.5s
            } else {
                setMessage(data.message || "Login failed");
            }
        } catch (err) {
            console.error(err);
            setMessage("An error occurred. Please try again.");
        }
    };

    return (
        <div className="login-container">

            { /* LOGIN TITLE */}
            <div className="login-title">
                Login
            </div>

            { /* LOGIN FORM */}
            <form className="login-form" onSubmit={handleSubmit}>
                <input
                    type="text"
                    placeholder="Username"
                    value={username}
                    onChange={(e) => setUsername(e.target.value)}
                />
                <input
                    type="password"
                    placeholder="Password"
                    value={password}
                    onChange={(e) => setPassword(e.target.value)}
                />
                <button type="submit">Sign In</button>
            </form>

            { /* LOGIN MESSAGE */}
            {message && <p className="login-message">{message}</p>}


        </div>
    )
}

export default LoginPage;
