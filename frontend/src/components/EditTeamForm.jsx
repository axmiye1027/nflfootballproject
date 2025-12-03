
import React, { useState, useEffect } from 'react';

import '../styles/editTeamForm.css'

function EditTeamForm({ team, onSubmit, onDelete, disabled })
{
    const [formData, setFormData] = React.useState({ ...team });

    useEffect(() => {
        setFormData({ ...team });
    }, [team]);

    const handleChange = (e) => {
        const { name, value } = e.target;
        setFormData(prev => ({ ...prev, [name]: value }));
    };

    const handleSubmit = (e) => {
        e.preventDefault();
        // Ensure numeric fields are numbers when sending to backend
        const payload = {
            ...formData,
            stadiumId: typeof formData.stadiumId === 'string' ? parseInt(formData.stadiumId) : formData.stadiumId,
            capacity: formData.capacity === '' ? null : Number(formData.capacity),
            yearOpened: formData.yearOpened === '' ? null : Number(formData.yearOpened)
        };

        console.debug('EditTeamForm submitting payload:', payload);

        try {
            const result = onSubmit(payload);
            // If onSubmit returns a promise, handle errors
            if (result && typeof result.then === 'function') {
                result.catch(err => console.error('Save failed (async):', err));
            }
        } catch (err) {
            console.error('Save failed (sync):', err);
        }
    };

    return (
        <div className="edit-team-container">
            <form onSubmit={handleSubmit} className="edit-team-form">
                <div className="form-row">
                    <label>Team Name:</label>
                    <input name="teamName" value={formData.teamName} onChange={handleChange} disabled={disabled} />
                </div>

                <div className="form-row">
                    <label>Stadium Name:</label>
                    <input name="stadiumName" value={formData.stadiumName} onChange={handleChange} disabled={disabled} />
                </div>

                <div className="form-row">
                    <label>Capacity:</label>
                    <input name="capacity" type="number" value={formData.capacity} onChange={handleChange} disabled={disabled} />
                </div>

                <div className="form-row">
                    <label>Location:</label>
                    <input name="location" value={formData.location} onChange={handleChange} disabled={disabled} />
                </div>

                <div className="form-row">
                    <label>Roof Type:</label>
                    <input name="roofType" value={formData.roofType} onChange={handleChange} disabled={disabled} />
                </div>

                <div className="form-row">
                    <label>Surface Type:</label>
                    <input name="surfaceType" value={formData.surfaceType} onChange={handleChange} disabled={disabled} />
                </div>

                <div className="form-row">
                    <label>Year Opened:</label>
                    <input name="yearOpened" type="number" value={formData.yearOpened} onChange={handleChange} disabled={disabled} />
                </div>

                <div className="form-row">
                    <label>Conference:</label>
                    <input name="conference" value={formData.conference} onChange={handleChange} disabled={disabled} />
                </div>

                <div className="form-row">
                    <label>Division:</label>
                    <input name="division" value={formData.division} onChange={handleChange} disabled={disabled} />
                </div>

                <div className="form-buttons">
                    <button type="submit" className="save-button" disabled={disabled}>Save Changes</button>
                    <button 
                        type="button" 
                        className="delete-button"
                        onClick={() => onDelete(team.stadiumId)} 
                        disabled={disabled}

                    >
                        Delete Team
                    </button>
                </div>
            </form>
        </div>
    )
}

export default EditTeamForm