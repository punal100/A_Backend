import React, { useState } from 'react';
import api from '../api/axios';

const PlayerDetails = () => {
    const [playerId, setPlayerId] = useState('');
    const [playerData, setPlayerData] = useState(null);
    const [inventory, setInventory] = useState([]);
    const [error, setError] = useState('');

    const handleFetchData = async () => {
        setError('');
        setPlayerData(null);
        setInventory([]);
        try {
            const dataResponse = await api.get(`/player/${playerId}/data`);
            setPlayerData(dataResponse.data);
            const inventoryResponse = await api.get(`/player/${playerId}/inventory`);
            setInventory(inventoryResponse.data.items || []);
        } catch (err) {
            setError('Player not found or error fetching data.');
        }
    };
    
    return (
        <div>
            <h2>Player Details</h2>
            <input type="text" value={playerId} onChange={(e) => setPlayerId(e.target.value)} placeholder="Enter Player ID" />
            <button onClick={handleFetchData}>Fetch Player Data</button>
            {error && <p style={{color: 'red'}}>{error}</p>}
            {playerData && (
                <div>
                    <h3>Player Data</h3>
                    <p>Level: {playerData.level}</p>
                    <p>XP: {playerData.xp}</p>
                </div>
            )}
            {inventory.length > 0 && (
                <div>
                    <h3>Inventory</h3>
                    <ul>
                        {inventory.map((item, index) => (
                            <li key={index}>{item}</li>
                        ))}
                    </ul>
                </div>
            )}
        </div>
    );
};

export default PlayerDetails;
