import express from "express";
import bcrypt from "bcrypt";
import jwt from "jsonwebtoken";
import dotenv from "dotenv";

dotenv.config();
const app = express()
app.use(express.json());

const users = [
    {
        username: "Sky",
        password: "dam"
    }
]

let refreshtk = []

function GenAccessToken(user){
    return jwt.sign(user, process.env.ACCESSTOKENSCRT, {expiresIn: '15s'})
}

app.post("/token", async (req, res) => {
    const refresh = req.body.token;
    if(!refresh) return res.status(401).send();
    if(!refreshtk.includes(refresh)) return res.status(403).send();
    jwt.verify(refresh, process.env.REFRESHTOKENSCRT, (err, user) => {
        if(err) return res.status(403).send();
        const accessToken = GenAccessToken({ name: user.name });

        return res.json({accessToken});
    })
});

app.delete("/logout", (req, res) => {
    refreshtk = refreshtk.filter(token => token !== req.body.token);
    res.status(204).send();
})

app.post("/login", async (req, res) => {
    const user = users.find(user => user.name === req.body.name);
    if(!user)
        return res.status(400).send();
    try {
        if (await bcrypt.compare(req.body.password, user.password)){
            const token = GenAccessToken(user);
            const refresh = jwt.sign(user, process.env.REFRESHTOKENSCRT);
            refreshtk.push(refresh);
            res.json({ accessToken: token, refreshToken: refresh });
        } else {
            res.send("notallowed")
        }
    } catch (error) {
        return res.status(500).send();
    }
});

app.post("/signup", async (req, res) => {
    try {
        const hashedpassword = await bcrypt.hash(req.body.password, 10);
        const user = { name: req.body.name, password: hashedpassword };
        users.push(user);
        res.status(201).send();
    } catch (error) {
        res.status(500).send();
    }
});

app.listen(3000)