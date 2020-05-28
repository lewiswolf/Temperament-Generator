const max = require('max-api');
const fs = require('fs');
const path = require('path')

function writeFile(file, filepath) {
    fs.writeFile(filepath, file, (err) => {
        err ? max.post(err) : max.post('The file has been saved!');
    });
}

max.addHandler('export-csv', (filepath, ...temperament) => {
    writeFile(`MIDI Note, MIDI Cents, Hz
${temperament.map((cents, i) => {
        return `${i}, ${cents * 100}, ${440 * Math.pow(2, (cents / 12))} \n`
    }).join('')}`, filepath)
})

max.addHandler('export-scl', (filepath, description, ...structure) => {
    let scala = structure.splice(3).map((cents) => {
        return (cents * 100) - (1200 * Math.log2(structure[0] / 440))
    });
    scala.push(1200 * Math.log2(structure[1] / 440));
    writeFile(`! ${path.basename(filepath)}
!
${description}
${scala.length}
!
${scala.map((cents) => {
        return `${cents} \n`
    }).join('')}! Created using the Temperament Generator by Lewis Wolf`, filepath)
})

max.addHandler('export-tun', (filepath, description, ...temperament) => {
    writeFile(`[Scale Begin]
Format = "AnaMark-TUN"
FormatVersion = 200
FormatSpecs = "http://www.mark-henning.de/eternity/tuningspecs.html"

[Info]
Name = "${filepath}"
ID = "ID_${path.basename(filepath, '.tun')}"
Filename = "${path.basename(filepath)}"
Description = "${description}"
Editor = "Temperament Generator by Lewis Wolf"

[Tuning]
${temperament.map((cents, i) => {
        return `note ${i} = ${cents !== 'null' ? Math.round((cents + 69) * 100) : 0} \n`
    }).join('')}
[Exact Tuning]
${temperament.map((cents, i) => {
        return `note ${i} = ${cents !== 'null' ? (cents + 69) * 100 : 0} \n`
    }).join('')}
[Scale End]`, filepath)

    /*
    functional tuning isn't as precise, and takes precedent over regular tuning, and was therefore not used

    [Functional Tuning]
    ${temperament.map(function (cents, i) {
            return `note ${i} = ${cents !== 'null' ? `*2 /1 %${cents} +0` : '*0 +0'} \n`
        }).join('')}
    */
})