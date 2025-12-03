
import React, { useState, useEffect } from 'react';

import '../styles/editTeamForm.css'

function EditTeamForm({ team, onSubmit, onDelete, disabled })
{
    const [formData, setFormData] = React.useState({
        ...team,
        souvenirs: team.souvenirs || []
    });

    useEffect(() => {
        setFormData({
            ...team,
            souvenirs: team.souvenirs || []
        });
    }, [team]);

    const handleChange = (e) => {
        const { name, value } = e.target;
        setFormData(prev => ({ ...prev, [name]: value }));
    };

    const handleSubmit = (e) => {
        e.preventDefault();
        onSubmit(formData);
    };

    const handleSouvenirChange = (index, field, value) => {
        setFormData(prev => {
            const updated = [...prev.souvenirs];
            updated[index] = { ...updated[index], [field]: value };
            return { ...prev, souvenirs: updated };
        });
    };

    const addSouvenir = () => {
        setFormData(prev => ({
            ...prev,
            souvenirs: [
                ...prev.souvenirs,
                {
                    souvenirId: -1,
                    stadiumId: prev.stadiumId, 
                    souvenirName: "",
                    souvenirPrice: 0.00
                }
            ]
        }));
    };

    const deleteSouvenir = (index) => {
        setFormData(prev => {
            const updated = [...prev.souvenirs];
            updated.splice(index, 1);
            return { ...prev, souvenirs: updated };
        });
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

                <div className="souvenir-section">
                    <h3>Souvenirs</h3>

                    {formData.souvenirs.map((souvenir, index) => (
                        <div key={index} className="souvenir-row">
                            <input
                                name="name"
                                placeholder="Name"
                                value={souvenir.souvenirName}
                                onChange={(e) => handleSouvenirChange(index, "souvenirName", e.target.value)}
                                disabled={disabled}
                            />

                            <input
                                type="number"
                                placeholder="Price"
                                step="0.01"
                                value={souvenir.souvenirPrice}
                                onChange={(e) => handleSouvenirChange(index, "souvenirPrice", parseFloat(e.target.value))}
                                disabled={disabled}
                            />

                            <button
                                type="button"
                                onClick={() => deleteSouvenir(index)}
                                disabled={disabled}
                                className="delete-souvenir-button"
                            >
                                X
                            </button>
                        </div>
                    ))}

                    <button 
                        type="button" 
                        onClick={addSouvenir} 
                        disabled={disabled}
                        className="add-souvenir-button"
                    >
                        + Add Souvenir
                    </button>
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