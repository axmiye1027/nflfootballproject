// admin.jsx

import { useState } from 'react'

import EditTeamSection from '../components/EditTeamSection'
import EditTeamForm    from '../components/EditTeamForm'
import AddTeamForm     from '../components/AddTeamForm'

import '../styles/admin.css'

function AdminPage({ stadiums, setStadiums })
{
    const [mode, setMode] = useState(null);

    const handleSave = async (updatedTeam) => {
        try {
            const res = await fetch(`http://localhost:18080/updateStadium`, {
            method: "POST",
            headers: { "Content-Type": "application/json" },
            body: JSON.stringify(updatedTeam),
            });

            const data = await res.json();
            console.log("Backend response:", data);

            // If update succeeded, refresh the stadium list from backend so UI shows latest data
            if (res.ok && (data["success"] == true || data.success === true)) {
                try {
                    const r = await fetch('http://localhost:18080/stadiums');
                    const json = await r.json();
                    const list = json.stadiums ? json.stadiums : json;
                    if (typeof setStadiums === 'function') setStadiums(list);
                } catch (err) {
                    console.error('Failed to refresh stadiums after update:', err);
                }
            }
        } catch (err) {
            console.error("Failed to save:", err);
        }
    };

    const handleDelete = async (id) => {
        try {
            const res = await fetch('http://localhost:18080/deleteStadium', {
                method: 'POST',
                headers: { 'Content-Type': 'application/json' },
                body: JSON.stringify({ stadiumId: id })
            });

            const data = await res.json();
            console.log('Delete response:', data);

            if (res.ok && (data.success === true || data["success"] == true)) {
                // refresh stadium list
                try {
                    const r = await fetch('http://localhost:18080/stadiums');
                    const json = await r.json();
                    const list = json.stadiums ? json.stadiums : json;
                    if (typeof setStadiums === 'function') setStadiums(list);
                } catch (err) {
                    console.error('Failed to refresh stadiums after delete:', err);
                }
            } else {
                console.error('Failed to delete stadium', data);
            }
        } catch (err) {
            console.error('Error calling delete endpoint:', err);
        }
    }

    return (
        <div className="admin-container">
            <div className="title">
                Admin Panel
            </div>

            {/* Edit Team Folder */}
            <div className="folder">
                <button
                    className="folder-header"
                    onClick={() => setMode(mode === "edit" ? null : "edit")}
                >
                    Edit Team {mode === "edit" ? "▲" : "▼"}
                </button>

                {mode === "edit" && (
                    <div className="folder-content">
                        <EditTeamSection
                            stadiums={stadiums}
                            onSave={(updatedTeam) => { 
                                console.log("Saving team:", updatedTeam)
                                handleSave(updatedTeam)
                            }}
                            onDelete={(id) => {
                                console.log("Delete", id)
                                handleDelete(id)
                            }}
                        />
                    </div>
                )}
            </div>

            {/* Add Team Folder */}
            <div className="folder">
                <button
                    className="folder-header"
                    onClick={() => setMode(mode === "add" ? null : "add")}
                >
                    Add Team/City {mode === "add" ? "▲" : "▼"}
                </button>

                {mode === "add" && (
                    <div className="folder-content">
                        <AddTeamForm onSubmit={data => console.log("Add", data)} />

                        
                    </div>
                )}
            </div>
        </div>
    );
}

export default AdminPage