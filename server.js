const express = require('express');
const fs = require('fs');
const path = require('path');
const cors = require('cors');

const app = express();
const SRV_PORT = 3000;

const DB_FILE = path.join(__dirname, 'users.txt');
const FILE_DIR = __dirname;

app.use(cors());
app.use(express.json());
app.use(express.static(__dirname));

function fetchUserRecords() {
  if (!fs.existsSync(DB_FILE)) return [];
  return fs.readFileSync(DB_FILE, 'utf8')
    .split('\n')
    .map(line => line.trim())
    .filter(Boolean)
    .map(line => {
      const [uName, uPass, uRole] = line.split(' ');
      return { uName, uPass, uRole };
    });
}

function saveUserRecords(usrArr) {
  const fileData = usrArr.map(user => `${user.uName} ${user.uPass} ${user.uRole}`).join('\n') + '\n';
  fs.writeFileSync(DB_FILE, fileData, 'utf8');
}

function generateAuthToken(uName, uRole) {
  return Buffer.from(`${uName}:${uRole}`).toString('base64');
}

function parseAuthToken(tok) {
  try {
    const [uName, uRole] = Buffer.from(tok, 'base64').toString('utf8').split(':');
    return { uName, uRole };
  } catch { return null; }
}

function verifyAuth(req, res, nxt) {
  const authHeader = req.headers['authorization'] || '';
  const cleanTok = authHeader.replace('Bearer ', '').trim();
  const parsed = parseAuthToken(cleanTok);
  if (!parsed) return res.status(401).json({ error: 'Auth failed' });
  req.authData = parsed;
  nxt();
}

function verifyAdmin(req, res, nxt) {
  if (req.authData?.uRole !== 'admin') return res.status(403).json({ error: 'Access denied' });
  nxt();
}

const ROOM_SIZES = [120, 110, 100, 90, 80, 70, 60, 55, 120, 110, 50, 45, 40, 35, 30, 45, 50, 40, 30, 50];

function checkLab(subj) {
  return subj.toLowerCase().includes('lab');
}

function canSchedule(allocs, curr, d, t, r, list, totalR) {
  const limit = ROOM_SIZES[r] ?? 120;
  if (checkLab(list[curr].subject) && limit > 50) return false;
  if (!checkLab(list[curr].subject) && limit <= 50) return false;

  for (let idx = 0; idx < curr; idx++) {
    if (allocs[idx].day === d && allocs[idx].period === t) {
      if (list[idx].teacher === list[curr].teacher) return false;
      if (list[idx].className === list[curr].className) return false;
      if (allocs[idx].room === r) return false;
    }
  }
  return true;
}

function performScheduling(curr, allocs, list, limitD, limitP, limitR) {
  if (curr === list.length) return true;
  for (let d = 0; d < limitD; d++) {
    for (let p = 0; p < limitP; p++) {
      for (let r = 0; r < limitR; r++) {
        if (canSchedule(allocs, curr, d, p, r, list, limitR)) {
          allocs[curr] = { day: d, period: p, room: r };
          if (performScheduling(curr + 1, allocs, list, limitD, limitP, limitR))
            return true;
          allocs[curr] = { day: -1, period: -1, room: -1 };
        }
      }
    }
  }
  return false;
}

function generateTextFormat(list, allocs, limitD, limitP) {
  const labels = ['Mon','Tue','Wed','Thu','Fri','Sat'];
  const uniqueSecs = [...new Set(list.map(item => item.className))].sort();

  let resultStr = '';

  for (const sec of uniqueSecs) {
    resultStr += `==================== GROUP: ${sec} ====================\n\n`;
    const tempGrid = {};
    for (let d = 0; d < limitD; d++) tempGrid[labels[d]] = {};

    for (let i = 0; i < list.length; i++) {
      if (list[i].className === sec) {
        const { day, period, room } = allocs[i];
        if (day !== -1) {
          tempGrid[labels[day]][period] =
            `${list[i].subject}(${list[i].teacher},R${room + 1})`;
        }
      }
    }

    for (let d = 0; d < limitD; d++) {
      const dLabel = labels[d];
      resultStr += `${dLabel}:\n`;
      const maxSlots = Math.max(limitP, 9);
      for (let p = 0; p < maxSlots; p++) {
        resultStr += `Slot ${p + 1} -> ${tempGrid[dLabel][p] || '---'}\n`;
      }
      resultStr += '-----------------------------\n';
    }
    resultStr += '\n';
  }
  return resultStr.trim() + '\n';
}

app.post('/api/login', (req, res) => {
  const { username, password } = req.body;
  if (!username || !password) return res.status(400).json({ error: 'Data missing' });

  const allUsrs = fetchUserRecords();
  const match = allUsrs.find(u => u.uName === username && u.uPass === password);
  if (!match) return res.status(401).json({ error: 'Wrong credentials' });

  res.json({ token: generateAuthToken(match.uName, match.uRole), role: match.uRole, username: match.uName });
});

app.post('/api/signup', (req, res) => {
  const { username, password } = req.body;
  if (!username || !password) return res.status(400).json({ error: 'Data missing' });
  if (password.length < 4) return res.status(400).json({ error: 'Passcode too short' });

  const allUsrs = fetchUserRecords();
  if (allUsrs.find(u => u.uName === username)) return res.status(409).json({ error: 'User exists' });

  allUsrs.push({ uName: username, uPass: password, uRole: 'user' });
  saveUserRecords(allUsrs);
  res.json({ message: 'Success' });
});

app.get('/api/users', verifyAuth, verifyAdmin, (req, res) => {
  const safeData = fetchUserRecords().map(({ uName, uRole }) => ({ username: uName, role: uRole }));
  res.json(safeData);
});

app.delete('/api/users/:targetUser', verifyAuth, verifyAdmin, (req, res) => {
  const target = req.params.targetUser;
  const allUsrs = fetchUserRecords();
  const targetIdx = allUsrs.findIndex(u => u.uName === target);
  if (targetIdx === -1) return res.status(404).json({ error: 'Not found' });
  if (allUsrs[targetIdx].uRole === 'admin') return res.status(403).json({ error: 'Cannot remove admin' });

  allUsrs.splice(targetIdx, 1);
  saveUserRecords(allUsrs);
  res.json({ message: 'Removed' });
});

app.get('/api/timetables', verifyAuth, (req, res) => {
  const { uName, uRole } = req.authData;
  const foundFiles = fs.readdirSync(FILE_DIR).filter(item => item.startsWith('timetable_') && item.endsWith('.txt'));

  const validFiles = uRole === 'admin' ? foundFiles : foundFiles.filter(item => item.startsWith(`timetable_${uName}`));

  const parsedList = validFiles.map(item => {
    const fStat = fs.statSync(path.join(FILE_DIR, item));
    return {
      name: item,
      owner: item.replace('timetable_', '').replace(/(_\d+)?\.txt$/, ''),
      date: fStat.mtime.toDateString(),
      size: fStat.size
    };
  });
  res.json(parsedList);
});

app.get('/api/timetables/:targetFile', verifyAuth, (req, res) => {
  const target = req.params.targetFile;
  const { uName, uRole } = req.authData;

  if (!/^timetable_[\w\-]+\.txt$/.test(target)) return res.status(400).json({ error: 'Bad file name' });
  if (uRole !== 'admin' && !target.startsWith(`timetable_${uName}`)) return res.status(403).json({ error: 'Access denied' });

  const fullPath = path.join(FILE_DIR, target);
  if (!fs.existsSync(fullPath)) return res.status(404).json({ error: 'Missing file' });

  res.json({ filename: target, content: fs.readFileSync(fullPath, 'utf8') });
});

app.post('/api/timetables/save', verifyAuth, (req, res) => {
  const { uName } = req.authData;
  let textData = req.body.content;
  let targetName = req.body.filename;
  
  if (!textData) return res.status(400).json({ error: 'Empty content' });

  if (!targetName) {
    const matched = fs.readdirSync(FILE_DIR).filter(item => item.startsWith(`timetable_${uName}`) && item.endsWith('.txt'));
    targetName = matched.length === 0 ? `timetable_${uName}.txt` : `timetable_${uName}_${matched.length + 1}.txt`;
  }

  if (!/^timetable_[\w\-]+\.txt$/.test(targetName)) return res.status(400).json({ error: 'Bad file name' });

  fs.writeFileSync(path.join(FILE_DIR, targetName), textData, 'utf8');
  res.json({ message: 'Saved successfully', filename: targetName });
});

app.delete('/api/timetables/:targetFile', verifyAuth, (req, res) => {
  const target = req.params.targetFile;
  const { uName, uRole } = req.authData;

  if (!/^timetable_[\w\-]+\.txt$/.test(target)) return res.status(400).json({ error: 'Bad file name' });
  if (uRole !== 'admin' && !target.startsWith(`timetable_${uName}`)) return res.status(403).json({ error: 'Access denied' });

  const fullPath = path.join(FILE_DIR, target);
  if (!fs.existsSync(fullPath)) return res.status(404).json({ error: 'Missing file' });

  fs.unlinkSync(fullPath);
  res.json({ message: 'Erased successfully' });
});

app.post('/api/generate', verifyAuth, (req, res) => {
  const reqList = req.body.lectures;
  const reqD = req.body.days || 6;
  const reqP = req.body.periods || 9;
  const reqR = req.body.rooms || 20;

  if (!reqList || !Array.isArray(reqList) || reqList.length === 0) return res.status(400).json({ error: 'Missing lectures list' });
  if (reqList.length > 20) return res.status(400).json({ error: 'Cannot exceed 20 classes' });

  const allocData = Array(reqList.length).fill(null).map(() => ({ day: -1, period: -1, room: -1 }));
  const isOk = performScheduling(0, allocData, reqList, reqD, reqP, Math.min(reqR, 20));

  if (!isOk) return res.status(422).json({ error: 'Could not create schedule with limits.' });

  const textRes = generateTextFormat(reqList, allocData, reqD, reqP);
  res.json({ assigned: allocData, content: textRes });
});

app.listen(SRV_PORT, () => {
  console.log(`Server live on port ${SRV_PORT}`);
});