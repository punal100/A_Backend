import React, { useState } from 'react';
import api from '../api/axios';

const LoginUser = () => {
    const [username, setUsername] = useState('');
    const [password, setPassword] = useState('');
    const [sessionData, setSessionData] = useState(null);
    const [error, setError] = useState('');

    const handleLogin = async (e) => {
        e.preventDefault();
        setError('');
        setSessionData(null);

        try {
            const response = await api.post('/login', { username, password });
            // The backend returns: { token, userId, roles, etc. }
            setSessionData(response.data);
            
            // Optional: Save to localStorage if you want persistence
            // localStorage.setItem('user', JSON.stringify(response.data));
        } catch (err) {
            setError(err.response?.data || 'Login Failed');
        }
    };

    return (
        <div>
            <h2>Login</h2>
            <form onSubmit={handleLogin}>
                <div>
                    <input 
                        type="text" 
                        placeholder="Username" 
                        value={username} 
                        onChange={(e) => setUsername(e.target.value)} 
                    />
                </div>
                <div>
                    <input 
                        type="password" 
                        placeholder="Password" 
                        value={password} 
                        onChange={(e) => setPassword(e.target.value)} 
                    />
                </div>
                <button type="submit">Login</button>
            </form>

            {error && <p style={{color: 'red'}}>{error}</p>}

            {sessionData && (
                <div style={{marginTop: '20px', padding: '10px', background: '#333', borderRadius: '5px'}}>
                    <h3 style={{color: '#4caf50'}}>Login Successful</h3>
                    <p><strong>User ID:</strong> {sessionData.userId}</p>
                    <p><strong>Token:</strong> {sessionData.token}</p>
                    <p><strong>Roles:</strong> {sessionData.roles?.join(', ')}</p>
                    <p style={{fontSize: '0.8em', color: '#aaa'}}>
                        Copy the ID and Token to use in Unreal Engine.
                    </p>
                </div>
            )}
        </div>
    );
};

export default LoginUser;