import React, { useState } from 'react';
import api from '../api/axios';

const RegisterUser = () => {
    const [username, setUsername] = useState('');
    const [password, setPassword] = useState('');
    const [message, setMessage] = useState('');

    const handleRegister = async (e) => {
        e.preventDefault();
        try {
            const response = await api.post('/register', { username, password });
            setMessage(response.data);
            setUsername('');
            setPassword('');
        } catch (error) {
            setMessage(error.response.data);
        }
    };

    return (
        <div>
            <h2>Register User</h2>
            <form onSubmit={handleRegister}>
                <div>
                    <label>Username:</label>
                    <input type="text" value={username} onChange={(e) => setUsername(e.target.value)} />
                </div>
                <div>
                    <label>Password:</label>
                    <input type="password" value={password} onChange={(e) => setPassword(e.target.value)} />
                </div>
                <button type="submit">Register</button>
            </form>
            {message && <p>{message}</p>}
        </div>
    );
};

export default RegisterUser;
