import { useState } from "react"

// button allowing user to submit files
function AddTeamForm()
{
    const [fileName, setFileName] = useState('');
    const [message, setMessage] = useState('');

    async function handleFile(e) {
    const f = e.target.files[0];
    if (!f) return;
    setFileName(f.name);
    try {
        const text = await f.text();
        // optional: validate JSON
        JSON.parse(text);

        // POST to backend
        // Use full URL if backend runs on a different port, e.g. http://localhost:8080/importStadium
        const res = await fetch('http://localhost:18080/importStadium', {
            method: 'POST',
            headers: { 'Content-Type': 'application/json' },
            body: text
        });

        const json = await res.json();
        if (!res.ok) {
            setMessage(`Import failed: ${json.message || res.statusText}`);
        } else {
            setMessage(`Import succeeded: ${json.message || 'OK'}`);
        }
        } catch (err) {
        setMessage('Invalid JSON or network error: ' + err.message);
        }
    }

    // DISPLAY
    return (
        <div>
        <label>
            Import stadium JSON:
            <input type="file" accept=".json,application/json" onChange={handleFile} />
        </label>
        <div>{fileName}</div>
        <div>{message}</div>
        </div>
    )
}

export default AddTeamForm