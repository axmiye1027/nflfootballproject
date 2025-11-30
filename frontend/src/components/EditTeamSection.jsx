import React, { useState } from "react";

import EditTeamForm from "./EditTeamForm";

import "../styles/editTeamSection.css";

function EditTeamSection({ stadiums, onSave, onDelete }) {
    const [selectedTeam, setSelectedTeam] = useState(null);

    const handleSelectChange = (e) => {
        const id = parseInt(e.target.value);
        const teamObj = stadiums.find((s) => s.stadiumId === id) || null;
        setSelectedTeam(teamObj);
    };

    return (
        <div className="edit-team-section">

            {/* Left: dropdown */}
            <div className="edit-left">
                <select
                className="team-dropdown"
                onChange={handleSelectChange}
                value={selectedTeam?.stadiumId || ""}
                >
                <option value="">Select a Team</option>
                {stadiums.map((team) => (
                    <option key={team.stadiumId} value={team.stadiumId}>
                    {team.teamName}
                    </option>
                ))}
                </select>
            </div>

            {/* Right: form */}
            <div className="edit-right">
                {selectedTeam && (
                <EditTeamForm
                    team={selectedTeam}
                    onSubmit={onSave}
                    onDelete={onDelete}
                    disabled={!selectedTeam}
                />
                )}
            </div>

        </div>
    );
}

export default EditTeamSection;