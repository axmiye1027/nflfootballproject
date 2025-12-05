// admin.jsx

import { useState } from 'react'

import EditTeamSection from '../components/EditTeamSection'
import EditTeamForm    from '../components/EditTeamForm'
import AddTeamForm     from '../components/AddTeamForm'

import '../styles/admin.css'

function AdminPage({ stadiums })
{
    const [mode, setMode] = useState("edit");

    const handleSave = async (updatedTeam) => {
        try {
            const res = await fetch(`http://localhost:18080/updateStadium`, {
                method: "POST",
                headers: { "Content-Type": "application/json" },
                body: JSON.stringify(updatedTeam),
            });

            const data = await res.json();
            console.log("Backend response:", data);
        } catch (err) {
            console.error("Failed to save:", err);
        }
    };

    const handleDelete = async (stadiumId) => {
        if (!window.confirm("Are you sure you want to delete this stadium?")) return;

        try {
            const res = await fetch(`http://localhost:18080/deleteStadium`, {
                method: "POST",
                headers: { "Content-Type": "application/json" },
                body: JSON.stringify({ stadiumId })  // send the id in the body
            });

            const data = await res.json();

            if (!data.success) {
                console.error("Failed to delete stadium:", data.message);
                return;
            }

            console.log("Deleted stadium:", data);

            // Update frontend state
            const index = stadiums.findIndex(s => s.stadiumId === stadiumId);
            if (index !== -1) stadiums.splice(index, 1);

        } catch (err) {
            console.error("Error deleting stadium:", err);
        }
    };


    const handleAddTeam = async (formData) => {
        try {
            const res = await fetch("http://localhost:18080/addStadium", {
                method: "POST",
                headers: { "Content-Type": "application/json" },
                body: JSON.stringify(formData),
            });

            const data = await res.json();
            console.log("Backend response:", data);

            if (!data.success) {
                alert("Failed to add team: " + (data.message || "Unknown error")); 
                return;
            }

            // Optionally refresh the stadium list or update local state
            alert("Team added successfully!");
        } catch (err) {
            console.error("Error adding team:", err);
            alert("Error adding team: " + err.message);
        }
    };

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
                        <AddTeamForm 
                            existingStadiums={stadiums}
                            onSubmit={handleAddTeam} />

                        
                    </div>
                )}
            </div>
        </div>
    );
}

export default AdminPage