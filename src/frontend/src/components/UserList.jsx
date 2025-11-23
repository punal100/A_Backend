import React, { useState, useEffect } from 'react';
import api from '../api/axios';

const UserList = () => {
    const [users, setUsers] = useState([]);
    const [error, setError] = useState('');

    useEffect(() => {
        const fetchUsers = async () => {
            try {
                const response = await api.get('/users');

                // DEFENSIVE CODING:
                // Ensure response.data is actually an array before setting it.
                // If backend sends null, we default to [].
                if (Array.isArray(response.data)) {
                    setUsers(response.data);
                } else {
                    console.warn("Backend returned non-array data:", response.data);
                    setUsers([]);
                }
            } catch (err) {
                console.error('Error fetching users:', err);
                setError('Failed to load user list.');
            }
        };

        fetchUsers();
    }, []);

    return (
        <div>
            <h2>User List</h2>

            {error && <p style={{color: 'red'}}>{error}</p>}

            {!error && users.length === 0 && <p>No users found (or Database is empty).</p>}

            <ul>
                {/* Check if users exists AND is an array before mapping.
                   Also use Optional Chaining (?.) on roles to prevent crashes.
                */}
                {Array.isArray(users) && users.map((user, index) => (
                    <li key={user.id || index}>
                        <strong>{user.username}</strong>
                        <span> - Roles: {user.roles?.join(', ') || 'None'}</span>
                    </li>
                ))}
            </ul>
        </div>
    );
};

export default UserList;
