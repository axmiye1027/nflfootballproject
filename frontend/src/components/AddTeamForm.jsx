import { useState } from "react";
import '../styles/editTeamForm.css';

function AddTeamForm({ existingStadiums = [], onSubmit }) {
    const [formData, setFormData] = useState({
        teamName: "",
        stadiumName: "",
        capacity: 0,
        location: "",
        roofType: "",
        surfaceType: "",
        yearOpened: 0,
        conference: "",
        division: "",
        souvenirs: [],
        distances: existingStadiums.map(s => ({ stadiumId: s.stadiumId, distance: 0 }))
    });

    const handleChange = (e) => {
        const { name, value } = e.target;
        setFormData(prev => ({ ...prev, [name]: value }));
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
                { souvenirId: -1, stadiumId: -1, souvenirName: "", souvenirPrice: 0.00 }
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

    const handleDistanceChange = (stadiumId, value) => {
        setFormData(prev => ({
            ...prev,
            distances: prev.distances.map(d => 
                d.stadiumId === stadiumId ? { ...d, distance: parseFloat(value) } : d
            )
        }));
    };

    const handleSubmit = (e) => {
        e.preventDefault();
        onSubmit(formData);
    };

    return (
        <div className="edit-team-container">
            <form className="edit-team-form" onSubmit={handleSubmit}>
                {/* Basic team info */}
                {["teamName","stadiumName","capacity","location","roofType","surfaceType","yearOpened","conference","division"].map(field => (
                    <div key={field} className="form-row">
                        <label>{field.charAt(0).toUpperCase() + field.slice(1)}:</label>
                        <input 
                            name={field} 
                            value={formData[field]} 
                            onChange={handleChange} 
                            type={field === "capacity" || field === "yearOpened" ? "number" : "text"} 
                        />
                    </div>
                ))}

                {/* Souvenirs */}
                <div className="souvenir-section">
                    <h3>Souvenirs</h3>
                    {formData.souvenirs.map((s, i) => (
                        <div key={i} className="souvenir-row">
                            <input
                                placeholder="Name"
                                value={s.souvenirName}
                                onChange={(e) => handleSouvenirChange(i, "souvenirName", e.target.value)}
                            />
                            <input
                                type="number"
                                placeholder="Price"
                                step="0.01"
                                value={s.souvenirPrice}
                                onChange={(e) => handleSouvenirChange(i, "souvenirPrice", parseFloat(e.target.value))}
                            />
                            <button type="button" onClick={() => deleteSouvenir(i)}>X</button>
                        </div>
                    ))}
                    <button type="button" onClick={addSouvenir}>+ Add Souvenir</button>
                </div>

                {/* Distances */}
                <div className="distance-section">
                    <h3>Distances to Other Stadiums</h3>
                    {formData.distances.map(d => {
                        const stadium = existingStadiums.find(s => s.stadiumId === d.stadiumId);
                        return (
                            <div key={d.stadiumId} className="form-row">
                                <label>{stadium?.stadiumName || "Unknown"}:</label>
                                <input 
                                    type="number" 
                                    step="0.1"
                                    value={d.distance} 
                                    onChange={(e) => handleDistanceChange(d.stadiumId, e.target.value)} 
                                />
                            </div>
                        )
                    })}
                </div>

                <div className="form-buttons">
                    <button type="submit" className="save-button">Add Team</button>
                </div>
            </form>
        </div>
    )
}

export default AddTeamForm;
