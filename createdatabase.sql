use requirements;
DROP TABLE IF EXISTS requirement;
DROP TABLE IF EXISTS software_requirement;
DROP TABLE IF EXISTS test_state_type;
DROP TABLE IF EXISTS layer_type;
DROP TABLE IF EXISTS project;
DROP TABLE IF EXISTS component;

CREATE TABLE
project(
    project_uid int(8) NOT NULL AUTO_INCREMENT,
    title varchar(24),
    description varchar(512),
    PRIMARY KEY (project_uid)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 AUTO_INCREMENT=1; 

CREATE TABLE
test_state_type(
    test_state_uid int(4) NOT NULL AUTO_INCREMENT,
    value varchar(10),
    PRIMARY KEY (test_state_uid)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 AUTO_INCREMENT=1; 

CREATE TABLE
layer_type(
    layer_uid int(4) NOT NULL AUTO_INCREMENT,
    value varchar(8),
    PRIMARY KEY (layer_uid)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 AUTO_INCREMENT=1; 

CREATE TABLE
component(
    component_uid int(8) NOT NULL AUTO_INCREMENT,
    title varchar(8),
    descrition varchar(8),
    PRIMARY KEY (component_uid)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 AUTO_INCREMENT=1; 

CREATE TABLE 
requirement(
    req_uid int(10) NOT NULL AUTO_INCREMENT,
    linkup int(10),
    title VARCHAR(128),
    description VARCHAR(512),
    updates int(10),
    layer int(4),
    created timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
    modified timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
    version int(8),
    project_ref int(8),
    PRIMARY KEY (req_uid),
    CONSTRAINT linkup_ibfk_1 FOREIGN KEY (linkup) REFERENCES requirement(req_uid) ON DELETE CASCADE ON UPDATE CASCADE,
    CONSTRAINT updates_ibfk_1 FOREIGN KEY (updates) REFERENCES requirement(req_uid) ON DELETE CASCADE ON UPDATE CASCADE,
    CONSTRAINT layer_ibfk_1 FOREIGN KEY (layer) REFERENCES layer_type(layer_uid) ON DELETE CASCADE ON UPDATE CASCADE,
    CONSTRAINT project_ibfk_1 FOREIGN KEY (project_ref) REFERENCES project(project_uid) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 AUTO_INCREMENT=1; 

CREATE TABLE
software_requirement(
    sreq_uid int(8) NOT NULL,
    component int(8),
    unit_test_state int(4),
    integration_test_state int(4),
    is_implemented boolean,
    PRIMARY KEY (sreq_uid),
    CONSTRAINT component_ibfk_1 FOREIGN KEY (component) REFERENCES component(component_uid) ON DELETE CASCADE ON UPDATE CASCADE,
    CONSTRAINT unit_test_ibfk_1 FOREIGN KEY (unit_test_state) REFERENCES test_state_type(test_state_uid) ON DELETE CASCADE ON UPDATE CASCADE,
    CONSTRAINT integration_test_ibfk_1 FOREIGN KEY (integration_test_state) REFERENCES test_state_type(test_state_uid) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 AUTO_INCREMENT=1; 


insert into test_state_type (value) values ('NA');
insert into test_state_type (value) values ('Passing');
insert into test_state_type (value) values ('Failing');
insert into test_state_type (value) values ('Uncovered');

insert into layer_type (value) values ('Project');
insert into layer_type (value) values ('System');
insert into layer_type (value) values ('Software');
insert into layer_type (value) values ('Hardware');


insert into project (title, description) values ('Viquina fluid handy', 'Device to control one output remotely');
